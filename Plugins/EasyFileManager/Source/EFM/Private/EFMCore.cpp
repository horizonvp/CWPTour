// Copyright Simone Campitelli 2019

#include "EFMCore.h"
#include "EFM.h"
// ~ File System
#include "Runtime/Core/Public/Misc/Paths.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "Runtime/Core/Public/HAL/PlatformFilemanager.h"
// ~ Audio System
#include "Runtime/Engine/Classes/Sound/SoundWave.h"
#include "Runtime/Engine/Public/Audio.h"
#include "Runtime/Engine/Public/VorbisAudioInfo.h"
#include "Developer/TargetPlatform/Public/Interfaces/IAudioFormat.h"
// ~ Texture and Image System
#include "Runtime/Engine/Classes/Engine/Texture2D.h"
#include "Runtime/Engine/Public/HighResScreenshot.h"
#include "Runtime/Engine/Public/ImageUtils.h"
#include "Runtime/ImageWrapper/Public/IImageWrapper.h"
#include "Runtime/ImageWrapper/Public/IImageWrapperModule.h"
#include "Runtime/Engine/Public/TextureResource.h"
// ~ Additional C++ FileSystem
#include <fstream>

#define PLATFORM_SLASH GetPlatformPathSlashCharacter()

DEFINE_LOG_CATEGORY(LogEFM);

UEFMBPLibrary::UEFMBPLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UEFMBPLibrary::CreateFile(FString Directory, FString Name, bool& bSuccess, UEFMFile*& NewCreatedFile)
{
	UE_LOG(LogEFM, Log, TEXT("Creating file %s"), *UEFMBPLibrary::CreateFullPath(Directory, Name));
	std::ofstream NewFile(std::string(TCHAR_TO_UTF8(*UEFMBPLibrary::CreateFullPath(Directory, Name))));
	if (NewFile)
	{
		bSuccess = true;
		NewCreatedFile = UEFMFile::CreateEFMFile(Directory, Name);
	}
	else
	{
		bSuccess = false;
		NewCreatedFile = nullptr;
	}
}

void UEFMBPLibrary::ReadFile(FString Directory, FString Name, bool& bSuccess, UEFMFile*& ReadFile)
{
	UE_LOG(LogEFM, Log, TEXT("Reading file %s"), *UEFMBPLibrary::CreateFullPath(Directory, Name));
	if (UEFMBPLibrary::FileExists(Directory, Name))
	{
		bSuccess = true;
		ReadFile = UEFMFile::CreateEFMFile(Directory, Name);
	}
	else
	{
		UE_LOG(LogEFM, Warning, TEXT("File %s was not found"), *Name);
		bSuccess = false;
	}
}

void UEFMBPLibrary::MoveFileObject(UEFMFile* File, FString AbsoluteDestination, bool& bSuccess)
{
	if (File)
	{
		std::rename(TCHAR_TO_ANSI(*File->GetFullPath()), TCHAR_TO_ANSI(*AbsoluteDestination));
		FString NewFileName = "";
		FString NewFileDirectory = "";
		UEFMBPLibrary::GetDirectoryAndFileFromString(AbsoluteDestination, NewFileDirectory, NewFileName);
		if (UEFMBPLibrary::FileExists(NewFileDirectory, NewFileName))
		{
			bSuccess = true;
			File->SetNewDirectory(NewFileName);
			File->SetNewFileName(NewFileDirectory);
			return;
		}
		else
		{
			bSuccess = false;
			return;
		}
	}
	else
	{
		bSuccess = false;
		return;
	}
}

void UEFMBPLibrary::RenameFileObject(UEFMFile* File, FString NewFileName, bool& bSuccess)
{
	FString PreviousExtension = File->GetExtension();
	FString NewFullPath = "";
	if (NewFileName.Contains(".", ESearchCase::IgnoreCase, ESearchDir::FromStart))
	{
		NewFullPath = File->GetDirectory() + NewFileName;
	}
	else
	{
		NewFullPath = File->GetDirectory() + NewFileName + "." + File->GetExtension();
	}
	UEFMBPLibrary::MoveFileObject(File, NewFullPath, bSuccess);
}

void UEFMBPLibrary::DeleteFileObject(UEFMFile* File, bool& bSuccess)
{
	if (File)
	{
		std::remove(TCHAR_TO_ANSI(*File->GetFullPath()));
		bSuccess = !UEFMBPLibrary::FileExists(File->GetDirectory(), File->GetName());
	}
	else
	{
		bSuccess = false;
		return;
	}
}

bool UEFMBPLibrary::DirectoryExists(FString DirectoryPath)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (PlatformFile.DirectoryExists(*DirectoryPath))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool UEFMBPLibrary::CreateDirectory(FString NewDirectoryPath)
{
	return FPlatformFileManager::Get().GetPlatformFile().CreateDirectory(*NewDirectoryPath);
}

bool UEFMBPLibrary::CreateDirectoryTree(FString NewDirectoryTreePath)
{
	return FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*NewDirectoryTreePath);
}

bool UEFMBPLibrary::CopyDirectory(FString SourceDirectory, FString DestinationDirectory, bool bOverwriteExisting)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (PlatformFile.CopyDirectoryTree(*DestinationDirectory, *SourceDirectory, bOverwriteExisting))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool UEFMBPLibrary::MoveDirectory(FString SourceDirectory, FString DestinationDirectory)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (PlatformFile.CopyDirectoryTree(*DestinationDirectory, *SourceDirectory, true))
	{
		if (PlatformFile.DeleteDirectoryRecursively(*SourceDirectory))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool UEFMBPLibrary::RenameDirectory(FString FullDirectoryPath, FString NewDirectoryName)
{
	int32 LastSlashIndex;
	FullDirectoryPath.FindLastChar(TCHAR('/'), LastSlashIndex); // Search for the last slash index
	if (LastSlashIndex == -1)
	{
		FullDirectoryPath.FindLastChar(TCHAR('\\'), LastSlashIndex);
	}
	return MoveDirectory(FullDirectoryPath, FullDirectoryPath.LeftChop(FullDirectoryPath.Len() - LastSlashIndex - 1) + NewDirectoryName);
}

bool UEFMBPLibrary::DeleteDirectory(FString Directory, bool bRecursively)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (bRecursively)
	{
		if (PlatformFile.DeleteDirectoryRecursively(*Directory))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if (PlatformFile.DeleteDirectory(*Directory))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

TArray<FString> UEFMBPLibrary::GetDirectoryContent(FString SourceDirectory, bool bFiles, bool bDirectories)
{
	TArray<FString> List;
	FString FinalSourceDirectory = SourceDirectory + "/*.*";
	IFileManager::Get().FindFiles(List, *FinalSourceDirectory, bFiles, bDirectories);
	return List;
}

FString UEFMBPLibrary::GetDirectoryFromString(FString InputString)
{
	int32 LastSlashIndex;
	InputString.FindLastChar(TCHAR('/'), LastSlashIndex); // Search for the last slash index
	if (LastSlashIndex == -1)
	{
		InputString.FindLastChar(TCHAR('\\'), LastSlashIndex);
	}
	return InputString.LeftChop(InputString.Len() - LastSlashIndex - 1);
}

FString UEFMBPLibrary::GetGameExecutableDirectory()
{
	return FString(FPlatformProcess::BaseDir());
}

FString UEFMBPLibrary::GetGameContentDirectory()
{
	return FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir());
}

FString UEFMBPLibrary::GetGameSavedDirectory()
{
	return FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir());
}

FString UEFMBPLibrary::GetGameDirectoryRoot()
{
	return FPaths::ConvertRelativePathToFull(FPaths::RootDir());
}

FString UEFMBPLibrary::GetGameUprojectRoot()
{
	FString ReturningDirectory = FPaths::GetProjectFilePath();
	int LastSlashIndex = 0;
	ReturningDirectory.FindLastChar('/', LastSlashIndex);
	return ReturningDirectory.Left(LastSlashIndex);
}

void UEFMBPLibrary::GetDirectoryAndFileFromString(FString InputString, FString& Directory, FString& File)
{
	int32 LastSlashIndex;
	InputString.FindLastChar(TCHAR('/'), LastSlashIndex); // Search for the last slash index
	if (LastSlashIndex == -1)
	{
		InputString.FindLastChar(TCHAR('\\'), LastSlashIndex);
	}
	Directory = InputString.LeftChop(InputString.Len() - LastSlashIndex - 1);
	File = InputString.RightChop(LastSlashIndex + 1);
}

FString UEFMBPLibrary::GetFileFromDirectoryString(FString Directory)
{
	int32 LastSlashIndex;
	Directory.FindLastChar(TCHAR('/'), LastSlashIndex); // Search for the last slash index
	if (LastSlashIndex == -1)
	{
		Directory.FindLastChar(TCHAR('\\'), LastSlashIndex);
	}
	return Directory.RightChop(LastSlashIndex);
}

FString UEFMBPLibrary::GetExtensionFromString(FString File)
{
	int32 LastSlashIndex;
	File.FindLastChar(TCHAR('.'), LastSlashIndex); // Search for the last dot index
	return File.RightChop(LastSlashIndex + 1);
}

FString UEFMBPLibrary::CreateFullPath(FString Directory, FString File)
{
#if PLATFORM_WINDOWS
	if (!Directory.EndsWith("\\", ESearchCase::Type::IgnoreCase) && !Directory.EndsWith("/", ESearchCase::Type::IgnoreCase))
	{
		return FString(Directory + "/" + File);
	}
	else
	{
		return FString(Directory + File);
	}
#else
	if (Directory.EndsWith("/", ESearchCase::Type::IgnoreCase))
	{
		return FString(Directory + "/" + File);
	}
	else
	{
		return FString(Directory + File);
	}
#endif
}

FString UEFMBPLibrary::GetPlatformPathSlashCharacter()
{
#if PLATFORM_WINDOWS
	return FString("\\");
#else
	return FString("/");
#endif
}

bool UEFMBPLibrary::FileExists(FString Directory, FString Name)
{
	if (FPaths::FileExists(CreateFullPath(Directory, Name)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void UEFMBPLibrary::CopyFileObject(UEFMFile* File, FString AbsoluteDestination, bool& bSuccess, UEFMFile*& CopiedFile)
{
	if (File)
	{
		std::ifstream  FileCopySource(TCHAR_TO_UTF8(*File->GetFullPath()), std::ios::binary);
		std::ofstream  FileCopyDestination(TCHAR_TO_UTF8(*AbsoluteDestination), std::ios::binary);
		FileCopyDestination << FileCopySource.rdbuf();
		FString NewFileName = "";
		FString NewFileDirectory = "";
		UEFMBPLibrary::GetDirectoryAndFileFromString(AbsoluteDestination, NewFileDirectory, NewFileName);
		CopiedFile = UEFMFile::CreateEFMFile(NewFileDirectory, NewFileName);
		if (CopiedFile)
		{
			bSuccess = true;
			return;
		}
	}
}

// ~~~ FILE CLASS ~~~

UEFMFile::UEFMFile()
{
	
}


UEFMFile* UEFMFile::CreateEFMFile(FString NewFileDirectory, FString NewFileName)
{
	UEFMFile* NewFile = NewObject<UEFMFile>();
	NewFile->Directory = NewFileDirectory;
	NewFile->Name = NewFileName;
	NewFile->Extension = UEFMBPLibrary::GetExtensionFromString(NewFileName);
	return NewFile;
}

void UEFMFile::SetNewDirectory(FString NewDirectory)
{
	Directory = NewDirectory;
}

void UEFMFile::SetNewFileName(FString NewFileName)
{
	Name = NewFileName;
	Extension = UEFMBPLibrary::GetExtensionFromString(NewFileName);
}

FString UEFMFile::GetFullPath()
{
	return UEFMBPLibrary::CreateFullPath(Directory, Name);
}

FString UEFMFile::GetName()
{
	return this->Name;
}

FString UEFMFile::GetExtension()
{
	return this->Extension;
}

FString UEFMFile::GetDirectory()
{
	return this->Directory;
}

void UEFMFile::AppendString(FString StringToAppend)
{
	TArray<FString> NewContent = this->GetStrings();
	NewContent.Add(StringToAppend);
	FFileHelper::SaveStringArrayToFile(NewContent, *GetFullPath(), FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), 0);
}

TArray<FString> UEFMFile::GetStrings()
{
	TArray<FString> FileContent;
	FFileHelper::LoadFileToStringArray(FileContent, *GetFullPath(), FFileHelper::EHashOptions::None);
	return FileContent;
}

FString UEFMFile::GetSingleString()
{
	FString FileContent = "";
	FFileHelper::LoadFileToString(FileContent, *GetFullPath(), FFileHelper::EHashOptions::None);
	return FileContent;
}

float UEFMFile::GetSizeInBytes()
{
	std::ifstream in(TCHAR_TO_ANSI(*this->GetFullPath()), std::ifstream::ate | std::ifstream::binary);
	return in.tellg();
}

float UEFMFile::GetSizeInMegaBytes()
{
	return (this->GetSizeInBytes()) / 1000000;
}

void UEFMFile::GetTexture(EImageConversionResult& ConversionResult, UTexture2D*& Texture)
{
	UTexture2D* ConvertedTexture = nullptr;
	FString TexturePath = GetFullPath();
	TArray<uint8> RawFileData;
	if (FFileHelper::LoadFileToArray(RawFileData, *TexturePath))
	{
		IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
		// Get Format
		EImageFormat DetectedFormat;
		if (this->Extension == "jpeg" || this->Extension == "jpg" || this->Extension == "jpe" || this->Extension == "jfif")
		{
			DetectedFormat = EImageFormat::JPEG;
			UE_LOG(LogEFM, Log, TEXT("Texture Conversion detected JPEG as format"));
		}
		else if (this->Extension == "png")
		{
			DetectedFormat = EImageFormat::PNG;
			UE_LOG(LogEFM, Log, TEXT("Texture Conversion detected PNG as format"));
		}
		else if (this->Extension == "bmp")
		{
			DetectedFormat = EImageFormat::BMP;
			UE_LOG(LogEFM, Log, TEXT("Texture Conversion detected BMP as format"));
		}
		else if (this->Extension == "ico")
		{
			DetectedFormat = EImageFormat::ICO;
			UE_LOG(LogEFM, Log, TEXT("Texture Conversion detected ICO as format"));
		}
		else if (this->Extension == "exr")
		{
			DetectedFormat = EImageFormat::EXR;
			UE_LOG(LogEFM, Log, TEXT("Texture Conversion detected EXR as format"));
		}
		else if (this->Extension == "icns")
		{
			DetectedFormat = EImageFormat::ICNS;
			UE_LOG(LogEFM, Log, TEXT("Texture Conversion detected ICNS as format"));
		}
		else
		{
			DetectedFormat = EImageFormat::Invalid;
			UE_LOG(LogEFM, Warning, TEXT("Texture Conversion invalid format detected. Will use invalid parameter for conversion."));
		}
		TSharedPtr<IImageWrapper>ImageWrapper = ImageWrapperModule.CreateImageWrapper(DetectedFormat);
		if (FFileHelper::LoadFileToArray(RawFileData, *TexturePath))
		{
			if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(RawFileData.GetData(), RawFileData.Num()))
			{
				TArray64<uint8> UncompressedBGRA = TArray64<uint8>();
				if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, UncompressedBGRA))
				{
					ConvertedTexture = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_B8G8R8A8);

					// Fill in the source data from the file
					void* TextureData = ConvertedTexture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
					FMemory::Memcpy(TextureData, UncompressedBGRA.GetData(), UncompressedBGRA.Num());
					ConvertedTexture->PlatformData->Mips[0].BulkData.Unlock();
					ConvertedTexture->UpdateResource();
				}
				else
				{
					UE_LOG(LogEFM, Error, TEXT("Texture conversion failure, Could not get raw data."));
					ConversionResult = EImageConversionResult::UnknownError;
					return;
				}
			}
			else
			{
				UE_LOG(LogEFM, Error, TEXT("Texture conversion failure, Could not get ImageWrapper."));
				ConversionResult = EImageConversionResult::UnknownError;
				return;
			}
		}
		else
		{
			UE_LOG(LogEFM, Error, TEXT("Texture conversion failure, file was not found."));
			ConversionResult = EImageConversionResult::FileNotFound;
			return;
		}
	}
	else
	{
		UE_LOG(LogEFM, Error, TEXT("Texture conversion failure, file was not found."));
		ConversionResult = EImageConversionResult::FileNotFound;
		return;
	}
	ConversionResult = EImageConversionResult::Success;
	Texture = ConvertedTexture;
}

USoundWave* UEFMFile::GetSound()
{
	return nullptr;
}

TArray<uint8> UEFMFile::GetBytes()
{
	TArray<uint8> ReturningArray;
	FFileHelper::LoadFileToArray(ReturningArray, *GetFullPath(), 0);
	return ReturningArray;
}

bool UEFMFile::WriteBytes(TArray<uint8> ByteDataToWrite)
{
	return FFileHelper::SaveArrayToFile(ByteDataToWrite, *GetFullPath());
}
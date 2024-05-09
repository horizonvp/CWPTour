// Copyright Simone Campitelli 2019

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "EFMCore.generated.h"

class USoundWave;

DECLARE_LOG_CATEGORY_EXTERN(LogEFM, Log, All);

UCLASS()
class UEFMBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

public:

	// ~ File creation and reading

	/**
	 * Creates a new empty file
	 * @param Directory - The directory where the file should be created
	 * @param Name - The file name, along with its extension
	 * @param bSuccess - Returns if the operation was successful
	 * @param NewCreatedFile - Returns the newly created file reference
	 */
	UFUNCTION(BlueprintPure, Category = "File|Creation")
		static void CreateFile(FString Directory, FString Name, bool& bSuccess, UEFMFile*& NewCreatedFile);
	/**
	 * Reads a file from disk
	 * @param Directory - The directory where the file is located
	 * @param Name - The file name, along with its extension
	 * @param bSuccess - Returns if the operation was successful
	 * @param NewCreatedFile - Returns the read file
	 */
	UFUNCTION(BlueprintPure, Category = "File|Reading")
		static void ReadFile(FString Directory, FString Name, bool& bSuccess, UEFMFile*& ReadFile);

	// ~ Functions for File System operations

	// ~ Files

	/**
	 * Checks if a file exists
	 * @param Directory - The directory where the file is located
	 * @param Name - The file name, along with its extension
	 * @return Returns if the specified file exists
	 */
	UFUNCTION(BlueprintPure, Category = "EFM|Utilities")
		static bool FileExists(FString Directory, FString Name);
	/**
	 * Copies a given file to a given destination
	 * @param File - The file to copy
	 * @param AbsoluteDestination - The absolute file destination (Directory + File + Extension)
	 * @param bSuccess - Returns if the operation was successful
	 * @param CopiedFile - The copied file reference
	 */
	UFUNCTION(BlueprintCallable, Category = "File|Operations")
		static void CopyFileObject(UEFMFile* File, FString AbsoluteDestination, bool& bSuccess, UEFMFile*& CopiedFile);
	/**
	 * Moves a given file to a given destination
	 * @param File - The file to move
	 * @param AbsoluteDestination - The absolute file destination (Directory + File + Extension)
	 * @param bSuccess - Returns if the operation was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "File|Operations")
		static void MoveFileObject(UEFMFile* File, FString AbsoluteDestination, bool& bSuccess);

	/**
	 * Renames a given file object
	 * @param File - The file to rename
	 * @param NewName - The new file name (add a .extension if you also want to change extension)
	 * @param bSuccess - Returns if the operation was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "File|Operations")
		static void RenameFileObject(UEFMFile* File, FString NewFileName, bool& bSuccess);

	/**
	 * Deletes a given file to a given destination
	 * @param File - The file to delete
	 * @param bSuccess - Returns if the operation was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "File|Operations")
		static void DeleteFileObject(UEFMFile* File, bool& bSuccess);

	// ~ Directories

	/**
	 * Checks if a directory exists
	 * @param DirectoryPath - The directory path to check
	 * @return Returns true if the directory exists
	 */
	UFUNCTION(BlueprintPure, Category = "Directories|Operations")
		static bool DirectoryExists(FString DirectoryPath);

	/**
	 * Creates a new directory in the specified path
	 * @param NewDirectoryPath - The path of the new directory
	 * @return Returns true if the directory has been created
	 */
	UFUNCTION(BlueprintCallable, Category = "Directories|Operations")
		static bool CreateDirectory(FString NewDirectoryPath);

	/**
	 * Creates the entire directory tree in the specified path
	 * @param NewDirectoryTreePath - The path of the new directory tree
	 * @return Returns true if the directory tree has been created
	 */
	UFUNCTION(BlueprintCallable, Category = "Directories|Operations")
		static bool CreateDirectoryTree(FString NewDirectoryTreePath);

	/**
	 * Copies a directory to a given destination
	 * @param SourceDirectory - The directory to copy
	 * @param DestinationDirectory - The path where the directory should be copied to
	 * @param bOverwriteExistingDirectory - Should we overwrite existing directories?
	 * @return Returns true if the operation was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "Directories|Operations")
		static bool CopyDirectory(FString SourceDirectory, FString DestinationDirectory, bool bOverwriteExistingDirectory = true);
	/**
	 * Moves a directory to a given destination
	 * @param SourceDirectory - The directory to move
	 * @param DestinationDirectory - The path where the directory should be copied to
	 * @return Returns true if the operation was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "Directories|Operations")
		static bool MoveDirectory(FString SourceDirectory, FString DestinationDirectory);

	/**
	 * Renames a given directory
	 * @param FullDirectoryPath - The full path to the directory to rename
	 * @param NewDirectoryName - The new name for the given directory
	 * @return Returns if the operation was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "Directories|Operations")
		static bool RenameDirectory(FString FullDirectoryPath, FString NewDirectoryName);

	/**
	 * Deletes a given directory
	 * @param Directory - The directory path to delete
	 * @param bRecursively - Should we recursively delete the directory?
	 * @return Returns true if the directory exists
	 */
	UFUNCTION(BlueprintCallable, Category = "Directories|Operations")
		static bool DeleteDirectory(FString Directory, bool bRecursively = true);
	/**
	 * Gets a list of files and/or folders of a given directory
	 * @param SourceDirectory - The directory where we should get our list
	 * @param bFiles - Should we return a list of files?
	 * @param bDirectories - Should we return a list of directories?
	 * @return Returns true if the directory exists
	 */
	UFUNCTION(BlueprintPure, Category = "Directories|Operations")
		static TArray<FString> GetDirectoryContent(FString SourceDirectory, bool bFiles = true, bool bDirectories = true);

	// ~ Utility Functions

	/**
	 * Gets directory path from a given string
	 * @param InputString - The string containing the directory
	 * @return The extracted directory path name
	 */
	UFUNCTION(BlueprintPure, Category = "EFM|Utilities")
		static FString GetDirectoryFromString(FString InputString);

	/**
	 * Gets the game executable directory (Binaries/PLATFORM)
	 * @return The game executable directory
	 */
	UFUNCTION(BlueprintPure, Category = "EFM|Utilities")
		static FString GetGameExecutableDirectory();

	/**
	 * Gets the game content directory (Game/Content)
	 * @return The game content directory
	 */
	UFUNCTION(BlueprintPure, Category = "EFM|Utilities")
		static FString GetGameContentDirectory();

	/**
	 * Gets the game saved directory (Game/Saved)
	 * @return The game saved directory
	 */
	UFUNCTION(BlueprintPure, Category = "EFM|Utilities")
		static FString GetGameSavedDirectory();

	/**
	 * Gets the game root directory (PLATFORMnoeditor)
	 * @return The game root directory
	 */
	UFUNCTION(BlueprintPure, Category = "EFM|Utilities")
		static FString GetGameDirectoryRoot();

	/**
	 * Gets the game uproject path (project.uproject)
	 * @return The game uproject path
	 */
	UFUNCTION(BlueprintPure, Category = "EFM|Utilities")
		static FString GetGameUprojectRoot();

	/**
	 * Gets directory path and file name from a given string
	 * @param InputString - The string containing directory and file name
	 * @param Directory - The extracted directory path name
	 * @param File - The extraced file name
	 */
	UFUNCTION(BlueprintPure, Category = "EFM|Utilities")
		static void GetDirectoryAndFileFromString(FString InputString, FString& Directory, FString& File);
	/**
	 * Gets the file name from a full path string
	 * @param Directory - The string containing directory and file name
	 * @return Returns the file name along with its extension
	 */
	UFUNCTION(BlueprintPure, Category = "EFM|Utilities")
		static FString GetFileFromDirectoryString(FString Directory);
	/**
	 * Gets the file name from a full path string
	 * @param File - The string containing the file name
	 * @return Returns the file extension
	 */
	UFUNCTION(BlueprintPure, Category = "EFM|Utilities")
		static FString GetExtensionFromString(FString File);

	UFUNCTION(BlueprintPure, Category = "EFM|Utilities")
		static FString CreateFullPath(FString Directory, FString Name);

	/**
	 * Gets the platform path slash character
	 * @return The platform path slash character
	 */
	UFUNCTION(BlueprintPure, Category = "EFM|Platform|Utilities")
		static FString GetPlatformPathSlashCharacter();

};

/** Enumeration used as result for file to image conversion */
UENUM(BlueprintType)
enum class EImageConversionResult : uint8
{
	Success,
	UnknownFormat,
	FileNotFound,
	UnknownError
};

/** Class rappresenting a file on disk */
UCLASS(BlueprintType)
class UEFMFile : public UObject
{

	GENERATED_BODY()

protected:

	/** The file name */
	UPROPERTY(BlueprintReadOnly, Category = "File|Information")
		FString Name;

	/** The file extension */
	UPROPERTY(BlueprintReadOnly, Category = "File|Information")
		FString Extension;

	/** The directory of the file */
	UPROPERTY(BlueprintReadOnly, Category = "File|Information")
		FString Directory;

public:

	/** Functions that creates an EFMFile, used only by the plugin and not the developers */
	static UEFMFile* CreateEFMFile(FString NewFileDirectory, FString NewFileName);

	/** Sets a new directory for this object */
	void SetNewDirectory(FString NewDirectory);
	/** Sets a new file name for this object */
	void SetNewFileName(FString NewFileName);

	// ~ Information Functions

	/** Gets the full path of this file */
	UFUNCTION(BlueprintPure, Category = "File|Information")
		FString GetFullPath();

	/** Gets the full path of this file */
	UFUNCTION()
		FString GetName();

	/** Gets the file extension */
	UFUNCTION()
		FString GetExtension();

	/** Gets the directory containing this file */
	UFUNCTION()
		FString GetDirectory();

	/** Get the file size in bytes */
	UFUNCTION(BlueprintPure, Category = "File|Information")
		float GetSizeInBytes();

	/** Get the file size in megabytes */
	UFUNCTION(BlueprintPure, Category = "File|Information")
		float GetSizeInMegaBytes();


	// ~ String Functions

	/**
	 * Writes a string to the file (you should use this only if this is a text file)
	 * @param StringToAppend - The string to append to the file
	 */
	UFUNCTION(BlueprintCallable, Category = "File|Data|Strings")
		void AppendString(FString StringToAppend);

	/**
	 * Reads the entire file lines and returns it in an array (you should use this only if this is a text file)
	 * @return all the file strings
	 */
	UFUNCTION(BlueprintPure, Category = "File|Data|Strings")
		TArray<FString> GetStrings();

	/** Gets all the file lines into one string */
	UFUNCTION(BlueprintPure, Category = "File|Data|Strings")
		FString GetSingleString();

	// ~ Image Functions

	/**
	 * Converts the file into a texture (you should use this only if this is an image file)
	 * @param ConversionResult - Output parameter that return if the operation was successful
	 * @param Texture - The converted image
	 */
	UFUNCTION(BlueprintPure, Category = "File|Data|Images")
		void GetTexture(EImageConversionResult& ConversionResult, UTexture2D*& Texture);

	// ~ Sound Functions

	/**
	 * Converts the file into a texture (you should use this only if this is an image file)
	 * @param ConversionResult - Output parameter that return if the operation was successful
	 * @param Texture - The converted image
	 */
	UFUNCTION(BlueprintPure, Category = "File|Data|Images")
		USoundWave* GetSound();

	// ~ Raw Functions

	/** Converts the file to a byte array */
	UFUNCTION(BlueprintPure, Category = "File|Data")
		TArray<uint8> GetBytes();

	/**
	 * Writes a byte array to file
	 * @param ByteDataToWrite - The byte array to write to the file
	 * @return if the byte array has been written successfully
	 */
	UFUNCTION(BlueprintCallable, Category = "File|Data")
		bool WriteBytes(TArray<uint8> ByteDataToWrite);

	UEFMFile();


};
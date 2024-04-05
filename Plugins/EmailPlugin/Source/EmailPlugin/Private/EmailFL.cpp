// Copyright 2022 DownToCode. All Rights Reserved.

#include "EmailFL.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include "Runtime/Core/Public/Modules/ModuleManager.h"
#include "SaveCaptureComponentLatent.h"
#include "Kismet/KismetStringLibrary.h"

/*
 * Latent methods
 */
void UEmailFL::SendEmail(UObject* WorldContextObject, FEmailDetails EmailDetails, EEmailType EmailService, ECharSet CharacterSet, ESecurityType SecurityType, EEmailResult& OutResult, FString& OutError, FLatentActionInfo LatentInfo)
{
	//Register call to latent function
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
		if (LatentActionManager.FindExistingAction<FSendEmail>(LatentInfo.CallbackTarget, LatentInfo.UUID) == NULL)
		{
			LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, new FSendEmail(EmailDetails , EmailService, CharacterSet, SecurityType, OutResult, OutError, LatentInfo));
		}
	}
}

void UEmailFL::SendEmailV2(UObject* WorldContextObject, UEmailDetailsObject* EmailDetailsObject, ECharSet CharacterSet, ESecurityType SecurityType, EEmailResult& OutResult, FString& OutError, FLatentActionInfo LatentInfo)
{
	FEmailDetails EmailDetails;
	EmailDetails.SenderEmail = EmailDetailsObject->SenderEmail;
	EmailDetails.Password = EmailDetailsObject->Password;
	EmailDetails.SenderName = EmailDetailsObject->SenderName;
	EmailDetails.ReceiverEmail = EmailDetailsObject->ReceiverEmail;
	EmailDetails.CC = EmailDetailsObject->CC;
	EmailDetails.BCC = EmailDetailsObject->BCC;
	EmailDetails.Subject = EmailDetailsObject->Subject;
	EmailDetails.Message = EmailDetailsObject->Message;
	EmailDetails.Attachments = EmailDetailsObject->Attachments;
	EmailDetails.bUseHTML = EmailDetailsObject->bUsesHTML;

	//Register call to latent function
	if (UWorld * World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
		if (LatentActionManager.FindExistingAction<FSendEmail>(LatentInfo.CallbackTarget, LatentInfo.UUID) == NULL)
		{
			LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, new FSendEmail(EmailDetails, EmailDetailsObject->EmailService, CharacterSet, SecurityType, OutResult, OutError, LatentInfo));
		}
	}
}

void UEmailFL::SendEmailV3(UObject* WorldContextObject, FEmailDetails EmailDetails, FString Username, FString ServerName, int32 Port, ECharSet CharacterSet, ESecurityType SecurityType, EEmailResult& OutResult, FString& OutError, struct FLatentActionInfo LatentInfo)
{
	//Register call to latent function
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
		if (LatentActionManager.FindExistingAction<FSendEmail>(LatentInfo.CallbackTarget, LatentInfo.UUID) == NULL)
		{
			LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, new FSendEmail(EmailDetails, Username, ServerName, Port, CharacterSet, SecurityType, OutResult, OutError, LatentInfo));
		}
	}
}

void UEmailFL::SendEmailV4(UObject* WorldContextObject, UEmailDetailsObject* EmailDetailsObject, FString Username, FString ServerName, int32 Port, ECharSet CharacterSet, ESecurityType SecurityType, EEmailResult& OutResult, FString& OutError, FLatentActionInfo LatentInfo)
{
	FEmailDetails EmailDetails;
	EmailDetails.SenderEmail = EmailDetailsObject->SenderEmail;
	EmailDetails.Password = EmailDetailsObject->Password;
	EmailDetails.SenderName = EmailDetailsObject->SenderName;
	EmailDetails.ReceiverEmail = EmailDetailsObject->ReceiverEmail;
	EmailDetails.CC = EmailDetailsObject->CC;
	EmailDetails.BCC = EmailDetailsObject->BCC;
	EmailDetails.Subject = EmailDetailsObject->Subject;
	EmailDetails.Message = EmailDetailsObject->Message;
	EmailDetails.Attachments = EmailDetailsObject->Attachments;
	EmailDetails.bUseHTML = EmailDetailsObject->bUsesHTML;

	//Register call to latent function
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
		if (LatentActionManager.FindExistingAction<FSendEmail>(LatentInfo.CallbackTarget, LatentInfo.UUID) == NULL)
		{
			LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, new FSendEmail(EmailDetails, Username, ServerName, Port, CharacterSet, SecurityType, OutResult, OutError, LatentInfo));
		}
	}
}

void UEmailFL::SaveCaptureComponent2D(UObject* WorldContextObject, class USceneCaptureComponent2D* SceneCaptureComponent2D, FString ImagePath, EEmailResult& OutResult, struct FLatentActionInfo LatentInfo)
{
	//Register call to latent function
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
		if (LatentActionManager.FindExistingAction<FSaveCaptureComponent2D>(LatentInfo.CallbackTarget, LatentInfo.UUID) == NULL)
		{
			LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, new FSaveCaptureComponent2D(SceneCaptureComponent2D, ImagePath, OutResult, LatentInfo));
		}
	}
}
/**
 *~Latent methods 
 */


/*
 * Utilities
 */
UEmailDetailsObject* UEmailFL::MakeEmailDetailsFromAsset(FName SelectedUser, FString SelectedUserInternal, UEmailDetailsAsset* EmailDetailsAsset, FString ReceiverEmail, TArray<FString> CC, TArray<FString> BCC, FString Subject, FString Message, TArray<FString> Attachments, bool bUsesHTML)
{
	FUserDetails Details = FUserDetails();
	for (int32 i = 0; i < EmailDetailsAsset->UserDetailsList.Num(); i++)
	{
		if (EmailDetailsAsset->UserDetailsList[i].User == SelectedUser.ToString())
		{
			Details = EmailDetailsAsset->UserDetailsList[i];
		}
	}

	UEmailDetailsObject* DetailObject = NewObject<UEmailDetailsObject>();
	DetailObject->Init(Details.Email, Details.Password, Details.SenderName, ReceiverEmail, CC, BCC, Subject, Message, Attachments, Details.EmailService, bUsesHTML);
	return DetailObject;
}

TArray<FName> UEmailFL::GetUsersFromEmailDetailsAsset(UEmailDetailsAsset* EmailDetailsAsset)
{
	TArray<FName> Users;
	for (FUserDetails Details : EmailDetailsAsset->UserDetailsList)
	{
		Users.Add(FName(*Details.User));
	}

	return Users;
}

UEmailDetailsObject* UEmailFL::MakeEmailDetails(FString SenderEmail, FString Password, FString SenderName, FString ReceiverEmail, TArray<FString> CC, TArray<FString> BCC, FString Subject, FString Message, TArray<FString> Attachments, EEmailType EmailService, bool bUsesHTML)
{
	UEmailDetailsObject* EmailDetailsObject = NewObject<UEmailDetailsObject>();
	EmailDetailsObject->Init(SenderEmail, Password, SenderName, ReceiverEmail, CC, BCC, Subject, Message, Attachments, EmailService, bUsesHTML);
	return EmailDetailsObject;
}

FString UEmailFL::GetHTMLForEmbeddedAttachement(FString Attachment, FVector2D Size, int32 NumSpacesBeforePic, int32 NumSpacesAfterPic)
{
	FString HTML;

	HTML += "<html>" LINE_TERMINATOR;
	HTML += "<head>" LINE_TERMINATOR;
	HTML += "</head>" LINE_TERMINATOR;
	HTML += "<body>" LINE_TERMINATOR;

	for (int32 i = 0; i < NumSpacesBeforePic; i++)
	{
		HTML += "</br>" LINE_TERMINATOR;
	}

	HTML += *FString::Printf(TEXT("<img width = %d height = %d id = \"1\" src = \"cid:%s\">" LINE_TERMINATOR), (int32)Size.X, (int32)Size.Y, *Attachment);

	for (int32 i = 0; i < NumSpacesAfterPic; i++)
	{
		HTML += "</br>" LINE_TERMINATOR;
	}
	HTML += "</body>" LINE_TERMINATOR;
	HTML += "</html>" LINE_TERMINATOR;

	return HTML;
}

FString UEmailFL::GetHTMLForOnlineEmbeddedAttachment(FString URL, FVector2D Size, int32 NumSpacesBeforePic, int32 NumSpacesAfterPic)
{
	FString HTML;

	HTML += "<html>" LINE_TERMINATOR;
	HTML += "<head>" LINE_TERMINATOR;
	HTML += "</head>" LINE_TERMINATOR;
	HTML += "<body>" LINE_TERMINATOR;

	for (int32 i = 0; i < NumSpacesBeforePic; i++)
	{
		HTML += "</br>" LINE_TERMINATOR;
	}

	HTML += *FString::Printf(TEXT("<img width = %d height = %d src = \"%s\">" LINE_TERMINATOR), (int32)Size.X, (int32)Size.Y, *URL);

	for (int32 i = 0; i < NumSpacesAfterPic; i++)
	{
		HTML += "</br>" LINE_TERMINATOR;
	}
	HTML += "</body>" LINE_TERMINATOR;
	HTML += "</html>" LINE_TERMINATOR;

	return HTML;
}

FString UEmailFL::GetIMGTagForEmbeddedAttachment(FString Attachment, FVector2D Size)
{
	return FString::Printf(TEXT("<img width = %d height = %d id = \"1\" src = \"cid:%s\">" LINE_TERMINATOR), (int32)Size.X, (int32)Size.Y, *Attachment);
}

FString UEmailFL::GetIMGTagForOnlineEmbeddedAttachment(FString URL, FVector2D Size)
{
	return FString::Printf(TEXT("<img width = %d height = %d src = \"%s\">" LINE_TERMINATOR), (int32)Size.X, (int32)Size.Y, *URL);
}

TArray<FString> UEmailFL::ConvertToArrayLines(FString String)
{
	TArray<FString> Lines;
	FString Line;
	TArray<TCHAR> CharArray = String.GetCharArray();
	for (int32 i = 0; i < CharArray.Num(); i++)
	{
		if (CharArray[i] == *LINE_TERMINATOR)
		{
			Lines.Add(Line);
			Line = "";
		}
		else
		{
			Line.AppendChar(CharArray[i]);
		}
	}

	Lines.Add(Line);
	return Lines;
}

FString UEmailFL::ReplaceDiacriticsCharacters(FString InString)
{
	TArray<FString> DiacriticsChars;
	DiacriticsChars.Add(UTF8_TO_TCHAR("0"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("1"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("2"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("3"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("4"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("5"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("6"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("7"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("8"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("9"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("_"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("_"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("_"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("_"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("_"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("_"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("_"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("_"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("_"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("a"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("A"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("b"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("B"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("C"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("c"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("D"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("d"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("E"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("e"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("F"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("f"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("g"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("h"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("i"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("i"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("j"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("J"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("k"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("K"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("l"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("L"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("M"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("m"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("n"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("N"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("O"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("o"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("P"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("p"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("q"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("Q"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("R"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("r"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("S"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("s"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("T"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("t"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("U"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("u"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("v"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("V"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("W"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("w"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("X"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("x"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("Y"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("y"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("Z"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("z"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("ª"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("µ"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("º"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("À"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("Á"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("Â"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("Ã"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("Ä"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("Å"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("Æ"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("Ç"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("È"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("È"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("É"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("Ê"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("Ê"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("Ë"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("Ë"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("Ì"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("Í"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("Î"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("Ï"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("Ð"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("Ð"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("Ñ"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("Ò"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("Ó"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("Ô"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("Õ"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("Õ"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("Ö"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("Ø"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("Ù"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("Ú"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("Û"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("Ü"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("Ý"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("Þ"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("Þ"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("ß"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("à"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("á"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("â"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("ã"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("ä"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("å"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("æ"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("ç"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("è"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("é"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("ê"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("ë"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("ì"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("í"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("î"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("ï"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("ð"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("ð"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("ñ"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("ò"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("ó"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("ô"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("ö"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("ø"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("ù"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("ú"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("û"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("ü"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("ý"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("ÿ"));
	DiacriticsChars.Add(UTF8_TO_TCHAR("ƒ"));
		
	TArray<FString> AltChars;
	AltChars.Add("0");
	AltChars.Add("1");
	AltChars.Add("2");
	AltChars.Add("3");
	AltChars.Add("4");
	AltChars.Add("5");
	AltChars.Add("6");
	AltChars.Add("7");
	AltChars.Add("8");
	AltChars.Add("9");
	AltChars.Add("_");
	AltChars.Add("_");
	AltChars.Add("_");
	AltChars.Add("_");
	AltChars.Add("_");
	AltChars.Add("_");
	AltChars.Add("_");
	AltChars.Add("_");
	AltChars.Add("_");
	AltChars.Add("a");
	AltChars.Add("A");
	AltChars.Add("b");
	AltChars.Add("B");
	AltChars.Add("C");
	AltChars.Add("c");
	AltChars.Add("D");
	AltChars.Add("d");
	AltChars.Add("E");
	AltChars.Add("e");
	AltChars.Add("F");
	AltChars.Add("f");
	AltChars.Add("g");
	AltChars.Add("h");
	AltChars.Add("i");
	AltChars.Add("i");
	AltChars.Add("j");
	AltChars.Add("J");
	AltChars.Add("k");
	AltChars.Add("K");
	AltChars.Add("l");
	AltChars.Add("L");
	AltChars.Add("M");
	AltChars.Add("m");
	AltChars.Add("n");
	AltChars.Add("N");
	AltChars.Add("O");
	AltChars.Add("o");
	AltChars.Add("P");
	AltChars.Add("p");
	AltChars.Add("q");
	AltChars.Add("Q");
	AltChars.Add("R");
	AltChars.Add("r");
	AltChars.Add("S");
	AltChars.Add("s");
	AltChars.Add("T");
	AltChars.Add("t");
	AltChars.Add("U");
	AltChars.Add("u");
	AltChars.Add("v");
	AltChars.Add("V");
	AltChars.Add("W");
	AltChars.Add("w");
	AltChars.Add("X");
	AltChars.Add("x");
	AltChars.Add("Y");
	AltChars.Add("y");
	AltChars.Add("Z");
	AltChars.Add("z");
	AltChars.Add("a");
	AltChars.Add("u");
	AltChars.Add("o");
	AltChars.Add("A");
	AltChars.Add("A");
	AltChars.Add("A");
	AltChars.Add("A");
	AltChars.Add("A");
	AltChars.Add("A");
	AltChars.Add("AE");
	AltChars.Add("C");
	AltChars.Add("E");
	AltChars.Add("E");
	AltChars.Add("E");
	AltChars.Add("E");
	AltChars.Add("E");
	AltChars.Add("E");
	AltChars.Add("E");
	AltChars.Add("I");
	AltChars.Add("I");
	AltChars.Add("I");
	AltChars.Add("I");
	AltChars.Add("D");
	AltChars.Add("D");
	AltChars.Add("N");
	AltChars.Add("O");
	AltChars.Add("O");
	AltChars.Add("O");
	AltChars.Add("O");
	AltChars.Add("O");
	AltChars.Add("O");
	AltChars.Add("O");
	AltChars.Add("U");
	AltChars.Add("U");
	AltChars.Add("U");
	AltChars.Add("U");
	AltChars.Add("Y");
	AltChars.Add("P");
	AltChars.Add("P");
	AltChars.Add("B");
	AltChars.Add("a");
	AltChars.Add("a");
	AltChars.Add("a");
	AltChars.Add("a");
	AltChars.Add("a");
	AltChars.Add("a");
	AltChars.Add("ae");
	AltChars.Add("c");
	AltChars.Add("e");
	AltChars.Add("e");
	AltChars.Add("e");
	AltChars.Add("e");
	AltChars.Add("i");
	AltChars.Add("i");
	AltChars.Add("i");
	AltChars.Add("i");
	AltChars.Add("o");
	AltChars.Add("o");
	AltChars.Add("n");
	AltChars.Add("o");
	AltChars.Add("o");
	AltChars.Add("o");
	AltChars.Add("o");
	AltChars.Add("o");
	AltChars.Add("u");
	AltChars.Add("u");
	AltChars.Add("u");
	AltChars.Add("u");
	AltChars.Add("y");
	AltChars.Add("y");
	AltChars.Add("f");

	const FString DiacriticsLine = UTF8_TO_TCHAR("0123456789_________aAbBCcDdEeFfghiijJkKlLMmnNOoPpqQRrSsTtUuvVWwXxYyZzªµºÀÁÂÃÄÅÆÇÈÈÉÊÊËËÌÍÎÏÐÐÑÒÓÔÕÕÖØÙÚÛÜÝÞÞßàáâãäåæçèéêëìíîïððñòóôöøùúûüýÿƒ");
	for(int32 i = 0; i < DiacriticsChars.Num(); i++)
	{
		const int32 Index = UKismetStringLibrary::FindSubstring(InString, DiacriticsChars[i], true, false, -1);
		if(Index != INDEX_NONE)
		{
			InString.ReplaceInline(*DiacriticsChars[i], *AltChars[i], ESearchCase::CaseSensitive);
		}
	}

	return InString;
}

bool UEmailFL::DoesFileExist(const FString Path)
{
	return FPaths::FileExists(Path);
}

bool UEmailFL::DeleteFile(FString FilePath)
{
	if (!FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*FilePath))
	{
		UE_LOG(LogEmailPlugin, Warning, TEXT("Could Not Find File"));
		return false;
	}

	return true;
}

FString UEmailFL::GetAndroidGamePath()
{
#if PLATFORM_ANDROID
	extern FString GFilePathBase;
	return GFilePathBase + FString("/UE4Game/") + FApp::GetProjectName() + FString("/");
#else
	return FString("");
#endif
}
/*
 *~Utilities
 */
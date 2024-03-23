/************************************************************************/
/* Author: YWT20                                                        */
/* Expected release year : 2021                                         */
/************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/DataTable.h"
#include "Policies/PrettyJsonPrintPolicy.h"
#include "Dom/JsonValue.h"
#include "Dom/JsonObject.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CustomJsonConverter.h"
#include "GenericPlatform/GenericPlatformHttp.h"
#include "JsonCsvToDataTableToStructBPLibrary.generated.h"

UENUM(BlueprintType)
enum class EEncodingOptions : uint8
{
	AutoDetect,
	ForceAnsi,
	ForceUnicode,
	ForceUTF8,
	ForceUTF8WithoutBOM
};

UCLASS()
class UJsonCsvToDataTableToStructBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()
public:
	template<typename InStructType>
	static bool StructToJsonObjectStringC(const InStructType& InStruct, FString& OutJsonString)
	{
		return CustomJsonConverter::UStructToJsonObjectString(InStruct, OutJsonString);
	};

	template<typename InStructType>
	static bool StructArrayToJsonObjectStringC(const TArray<InStructType>& InStruct, FString& OutJsonString)
	{
		FString tempString = "";
		OutJsonString = "[\n\r";
		for (int32 i = 0; i < InStruct.Num(); i++)
		{
			StructToJsonObjectStringC(InStruct[i], tempString);
			OutJsonString += tempString;

			if (i < InStruct.Num() - 1) OutJsonString += ",\n\r";
		}
		OutJsonString += "\n\r]";
		return true;
	};

	template<typename OutStructType>
	static bool JsonObjectStringToStructC(const FString& JsonString, OutStructType* OutStruct)
	{
		return CustomJsonConverter::JsonObjectStringToUStruct(JsonString, OutStruct);
	};

	template<typename OutStructType>
	static bool JsonArrayStringToUStructC(const FString& JsonString, TArray<OutStructType>* OutStructArray)
	{
		return CustomJsonConverter::JsonArrayStringToUStruct(JsonString, OutStructArray);
	};

	/**
	 * Returns a percent-encoded version of the passed in string
	 *
	 * @param UnencodedString The unencoded string to convert to percent-encoding
	 * @return The percent-encoded string
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Json or csv to datatable to struct | V2")
	static FString UrlEncode(const FString UnencodedString) 
	{
		return FGenericPlatformHttp::UrlEncode(UnencodedString);
	};

	UFUNCTION(BlueprintCallable, Category = "Json or csv to datatable to struct | V2", CustomThunk, meta = (CustomStructureParam = "Struct"))
		static bool StructToJsonObjectStringV2(FString& OutJsonString, const UStruct* Struct);
	static bool Generic_StructToJsonObjectStringV2(FString& OutJsonString, const UStruct* StructDefinition, const void* Struct);
	DECLARE_FUNCTION(execStructToJsonObjectStringV2) {
		P_GET_PROPERTY_REF(FStrProperty, OutJsonString);

		Stack.StepCompiledIn<FStructProperty>(NULL);
		void* InStruct = Stack.MostRecentPropertyAddress;

		P_FINISH;
		bool bSuccess = false;

		FStructProperty* StructProp = CastField<FStructProperty>(Stack.MostRecentProperty);
		if (StructProp && InStruct)
		{
			P_NATIVE_BEGIN;
			bSuccess = Generic_StructToJsonObjectStringV2(OutJsonString, StructProp->Struct, InStruct);
			P_NATIVE_END;
		}
		*(bool*)RESULT_PARAM = bSuccess;
	}

	UFUNCTION(BlueprintCallable, Category = "Json or csv to datatable to struct | V2", CustomThunk, meta = (ArrayParm = "StructArray"))
		static bool StructArrayToJsonObjectStringV2(FString& OutJsonString, const TArray<UScriptStruct*> StructArray);
	static bool Generic_StructArrayToJsonObjectStringV2(FString& OutJsonString, FArrayProperty* StructDefinition, void* Struct);
	DECLARE_FUNCTION(execStructArrayToJsonObjectStringV2) {
		P_GET_PROPERTY_REF(FStrProperty, OutJsonString);

		Stack.MostRecentProperty = nullptr;
		Stack.StepCompiledIn<FArrayProperty>(NULL);
		void* SrcArrayAddr = Stack.MostRecentPropertyAddress;
		FArrayProperty* SrcArrayProperty = CastField<FArrayProperty>(Stack.MostRecentProperty);

		if (!SrcArrayProperty)
		{
			Stack.bArrayContextFailed = true;
			return;
		}

		P_FINISH;
		bool bSuccess = false;

		P_NATIVE_BEGIN;
		bSuccess = Generic_StructArrayToJsonObjectStringV2(OutJsonString, SrcArrayProperty, SrcArrayAddr);
		P_NATIVE_END;

		*(bool*)RESULT_PARAM = bSuccess;
	}

	UFUNCTION(BlueprintCallable, Category = "Json or csv to datatable to struct | V2", CustomThunk, meta = (CustomStructureParam = "Struct"))
		static bool JsonStringToStructV2(FString InJsonString, UStruct*& Struct);
	static bool Generic_JsonStringToStructV2(FString InJsonString, UStruct* StructDefinition, void* OutPtr);
	DECLARE_FUNCTION(execJsonStringToStructV2) {
		P_GET_PROPERTY(FStrProperty, InJsonString);

		Stack.StepCompiledIn<FStructProperty>(NULL);
		void* OutRowPtr = Stack.MostRecentPropertyAddress;

		P_FINISH;
		bool bSuccess = false;

		FStructProperty* StructProp = CastField<FStructProperty>(Stack.MostRecentProperty);
		if (StructProp && OutRowPtr)
		{
			P_NATIVE_BEGIN;
			bSuccess = Generic_JsonStringToStructV2(InJsonString, StructProp->Struct, OutRowPtr);
			P_NATIVE_END;
		}
		*(bool*)RESULT_PARAM = bSuccess;
	}

	UFUNCTION(BlueprintCallable, Category = "Json or csv to datatable to struct | V2", CustomThunk, meta = (ArrayParm = "StructArray"))
		static bool JsonStringToStructArrayV2(FString InJsonString, TArray<UScriptStruct*>& StructArray);
	static bool Generic_JsonStringToStructArrayV2(FString InJsonString, FArrayProperty* StructDefinition, void* Struct);
	DECLARE_FUNCTION(execJsonStringToStructArrayV2) {
		P_GET_PROPERTY(FStrProperty, InJsonString);

		Stack.MostRecentProperty = nullptr;
		Stack.StepCompiledIn<FArrayProperty>(NULL);
		void* SrcArrayAddr = Stack.MostRecentPropertyAddress;
		FArrayProperty* SrcArrayProperty = CastField<FArrayProperty>(Stack.MostRecentProperty);

		if (!SrcArrayProperty)
		{
			Stack.bArrayContextFailed = true;
			return;
		}

		P_FINISH;
		bool bSuccess = false;

		if (SrcArrayAddr)
		{
			P_NATIVE_BEGIN;
			bSuccess = Generic_JsonStringToStructArrayV2(InJsonString, SrcArrayProperty, SrcArrayAddr);
			P_NATIVE_END;
		}
		*(bool*)RESULT_PARAM = bSuccess;
	}



	UFUNCTION(BlueprintCallable, Category = "Json or csv to datatable to struct | V1")
		static bool LoadFileToString(const FString& FileAbsolutePath, FString& Content);

	UFUNCTION(BlueprintCallable, Category = "Json or csv to datatable to struct | V1")
		static bool SaveStringToFile(const FString& FileAbsolutePath, const FString& Content, EEncodingOptions EncodingOptions);

	UFUNCTION(BlueprintCallable, Category = "Json or csv to datatable to struct | V1")
		static UDataTable* ReadJsonToDataTable(const FString& InData, UScriptStruct* TableType);

	UFUNCTION(BlueprintCallable, Category = "Json or csv to datatable to struct | V1")
		static UDataTable* CreateDataTable(const FString& InDataTableName, UScriptStruct* TableType);

	UFUNCTION(BlueprintCallable, Category = "Json or csv to datatable to struct | V1")
		static bool ClearDataTable(UDataTable* InDataTable);

	UFUNCTION(BlueprintCallable, Category = "Json or csv to datatable to struct | V1")
		static UDataTable* ReadCsvToDataTable(const FString& InData, UScriptStruct* TableType);

	UFUNCTION(BlueprintCallable, Category = "Json or csv to datatable to struct | V1", CustomThunk, meta = (CustomStructureParam = "OutRow"))
		static bool GetDataTableFirstRow(UDataTable* InDataTable, FTableRowBase& OutRow);
	static bool Generic_GetDataTableFirstRow(UDataTable* InDataTable, UScriptStruct* TableType, void* OutRowPtr);
	DECLARE_FUNCTION(execGetDataTableFirstRow) {
		P_GET_OBJECT(UDataTable, InDataTable);

		Stack.StepCompiledIn<FStructProperty>(NULL);
		void* OutRowPtr = Stack.MostRecentPropertyAddress;

		P_FINISH;
		bool bSuccess = false;

		FStructProperty* StructProp = CastField<FStructProperty>(Stack.MostRecentProperty);
		if (StructProp && OutRowPtr)
		{
			UScriptStruct* OutputType = StructProp->Struct;

			P_NATIVE_BEGIN;
			bSuccess = Generic_GetDataTableFirstRow(InDataTable, OutputType, OutRowPtr);
			P_NATIVE_END;
		}
		*(bool*)RESULT_PARAM = bSuccess;
	}

	UFUNCTION(BlueprintCallable, Category = "Json or csv to datatable to struct | V1")
		static bool RemoveDataTableRow(UDataTable* InDataTable, const FName RowName);

	UFUNCTION(BlueprintCallable, Category = "Json or csv to datatable to struct | V1", CustomThunk, meta = (CustomStructureParam = "OutRow"))
		static bool AddDataTableRow(UDataTable* InDataTable, const FName RowName, const FTableRowBase& OutRow);
	static bool Generic_AddDataTableRow(UDataTable* InDataTable, const FName RowName, void* OutRowPtr);
	DECLARE_FUNCTION(execAddDataTableRow) {
		P_GET_OBJECT(UDataTable, InDataTable);
		P_GET_PROPERTY_REF(FNameProperty, RowName);

		Stack.StepCompiledIn<FStructProperty>(NULL);
		void* OutRowPtr = Stack.MostRecentPropertyAddress;

		P_FINISH;
		bool bSuccess = false;

		FStructProperty* StructProp = CastField<FStructProperty>(Stack.MostRecentProperty);
		if (StructProp && OutRowPtr)
		{
			UScriptStruct* OutputType = StructProp->Struct;

			P_NATIVE_BEGIN;
			bSuccess = Generic_AddDataTableRow(InDataTable, RowName, OutRowPtr);
			P_NATIVE_END;
		}
		*(bool*)RESULT_PARAM = bSuccess;
	}

	UFUNCTION(BlueprintCallable, Category = "Json or csv to datatable to struct | V1", CustomThunk, meta = (CustomStructureParam = "Struct"))
		static bool StructToJsonObjectString(FString& OutJsonString, const UScriptStruct* Struct);
	static bool Generic_StructToJsonObjectString(FString& OutJsonString, UScriptStruct* StructDefinition, void* Struct);
	DECLARE_FUNCTION(execStructToJsonObjectString) {
		P_GET_PROPERTY_REF(FStrProperty, OutJsonString);

		Stack.StepCompiledIn<FStructProperty>(NULL);
		void* InStruct = Stack.MostRecentPropertyAddress;

		P_FINISH;
		bool bSuccess = false;

		FStructProperty* StructProp = CastField<FStructProperty>(Stack.MostRecentProperty);
		if (StructProp && InStruct)
		{
			UScriptStruct* StructType = StructProp->Struct;

			P_NATIVE_BEGIN;
			bSuccess = Generic_StructToJsonObjectString(OutJsonString, StructType, InStruct);
			P_NATIVE_END;
		}
		*(bool*)RESULT_PARAM = bSuccess;
	}

	UFUNCTION(BlueprintCallable, Category = "Json or csv to datatable to struct | V1", CustomThunk, meta = (ArrayParm = "StructArray"))
		static bool StructArrayToJsonObjectString(FString& OutJsonString, const TArray<UScriptStruct*> StructArray);
	static bool Generic_StructArrayToJsonObjectString(FString& OutJsonString, FArrayProperty* StructDefinition, void* Struct);
	DECLARE_FUNCTION(execStructArrayToJsonObjectString) {
		P_GET_PROPERTY_REF(FStrProperty, OutJsonString);

		Stack.MostRecentProperty = nullptr;
		Stack.StepCompiledIn<FArrayProperty>(NULL);
		void* SrcArrayAddr = Stack.MostRecentPropertyAddress;
		FArrayProperty* SrcArrayProperty = CastField<FArrayProperty>(Stack.MostRecentProperty);

		if (!SrcArrayProperty)
		{
			Stack.bArrayContextFailed = true;
			return;
		}

		P_FINISH;
		bool bSuccess = false;

		P_NATIVE_BEGIN;
		bSuccess = Generic_StructArrayToJsonObjectString(OutJsonString, SrcArrayProperty, SrcArrayAddr);
		P_NATIVE_END;

		*(bool*)RESULT_PARAM = bSuccess;
	}

	UFUNCTION(BlueprintCallable, Category = "Json or csv to datatable to struct | V1", CustomThunk, meta = (CustomStructureParam = "OutRow"))
		static bool JsonStringToStruct(FString InJsonString, FTableRowBase& OutRow);
	static bool Generic_JsonStringToStruct(FString InJsonString, UScriptStruct* TableType, void* OutRowPtr);
	DECLARE_FUNCTION(execJsonStringToStruct) {
		P_GET_PROPERTY(FStrProperty, InJsonString);

		Stack.StepCompiledIn<FStructProperty>(NULL);
		void* OutRowPtr = Stack.MostRecentPropertyAddress;

		P_FINISH;
		bool bSuccess = false;

		FStructProperty* StructProp = CastField<FStructProperty>(Stack.MostRecentProperty);
		if (StructProp && OutRowPtr)
		{
			UScriptStruct* OutputType = StructProp->Struct;

			P_NATIVE_BEGIN;
			bSuccess = Generic_JsonStringToStruct(InJsonString, OutputType, OutRowPtr);
			P_NATIVE_END;
		}
		*(bool*)RESULT_PARAM = bSuccess;
	}

	UFUNCTION(BlueprintCallable, Category = "Json or csv to datatable to struct | V1", CustomThunk, meta = (ArrayParm = "OutRows"))
		static bool JsonStringToStructArray(FString InJsonString, TArray<FTableRowBase>& OutRows);
	static bool Generic_JsonStringToStructArray(FString InJsonString, FArrayProperty* TableType, void* OutRowPtr);
	DECLARE_FUNCTION(execJsonStringToStructArray) {
		P_GET_PROPERTY(FStrProperty, InJsonString);

		Stack.MostRecentProperty = nullptr;
		Stack.StepCompiledIn<FArrayProperty>(NULL);
		void* SrcArrayAddr = Stack.MostRecentPropertyAddress;
		FArrayProperty* SrcArrayProperty = CastField<FArrayProperty>(Stack.MostRecentProperty);

		if (!SrcArrayProperty)
		{
			Stack.bArrayContextFailed = true;
			return;
		}

		P_FINISH;
		bool bSuccess = false;

		if (SrcArrayAddr)
		{
			P_NATIVE_BEGIN;
			bSuccess = Generic_JsonStringToStructArray(InJsonString, SrcArrayProperty, SrcArrayAddr);
			P_NATIVE_END;
		}
		*(bool*)RESULT_PARAM = bSuccess;
	}


	UFUNCTION(BlueprintCallable, Category = "Json or csv to datatable to struct | V1")
		static bool WriteDataTableToCsv(UDataTable* InDataTable, FString& ExportedText);

	UFUNCTION(BlueprintCallable, Category = "Json or csv to datatable to struct | V1")
		static bool WriteDataTableToJson(UDataTable* InDataTable, FString& ExportedText);

	UFUNCTION(BlueprintCallable, Category = "Json or csv to datatable to struct | V1")
		static void CopyMessageToClipboard(FString text);

	UFUNCTION(BlueprintCallable, Category = "Json or csv to datatable to struct | V1")
		static FString PasteMessageFromClipboard();
};

class UDataTable;
enum class EDataTableExportFlags : uint8;

template <class CharType> struct TPrettyJsonPrintPolicy;

// forward declare JSON writer
template <class CharType>
struct TPrettyJsonPrintPolicy;
template <class CharType, class PrintPolicy>
class TJsonWriter;

namespace DataTableJSONUtils
{
	/** Returns what string is used as the key/name field for a data table */
	FString YGetKeyFieldName(const UDataTable& InDataTable);
}


/**
 * Class to serialize a DataTable to a TJsonWriter.
 */
template<typename CharType = TCHAR>
class TYDataTableExporterJSON
{
public:
	typedef TJsonWriter<CharType, TPrettyJsonPrintPolicy<CharType>> FDataTableJsonWriter;

	TYDataTableExporterJSON(const EDataTableExportFlags InDTExportFlags, TSharedRef<FDataTableJsonWriter> InJsonWriter);

	~TYDataTableExporterJSON();

	/** Writes the data table out as an array of objects */
	bool WriteTable(const UDataTable& InDataTable);

	/** Writes the data table out as a named object with each row being a sub value on that object */
	bool WriteTableAsObject(const UDataTable& InDataTable);

	/** Writes out a single row */
	bool WriteRow(const UScriptStruct* InRowStruct, const void* InRowData, const FString* FieldToSkip = nullptr);

	/** Writes the contents of a single row */
	bool WriteStruct(const UScriptStruct* InStruct, const void* InStructData, const FString* FieldToSkip = nullptr);

protected:
	bool WriteStructEntry(const void* InRowData, const FProperty* InProperty, const void* InPropertyData);

	bool WriteContainerEntry(const FProperty* InProperty, const void* InPropertyData, const FString* InIdentifier = nullptr);

	EDataTableExportFlags DTExportFlags;
	TSharedRef<FDataTableJsonWriter> JsonWriter;
	bool bJsonWriterNeedsClose;
};

/**
 * TCHAR-specific instantiation of TDataTableExporterJSON that has a convenience constructor to write output to an FString instead of an external TJsonWriter
 */
class FYDataTableExporterJSON : public TYDataTableExporterJSON<TCHAR>
{
public:
	using TYDataTableExporterJSON<TCHAR>::TYDataTableExporterJSON;
	// 
	FYDataTableExporterJSON(const EDataTableExportFlags InDTExportFlags, FString& OutExportText);
};

class FYDataTableImporterJSON
{
public:
	FYDataTableImporterJSON(UDataTable& InDataTable, const FString& InJSONData, TArray<FString>& OutProblems);

	~FYDataTableImporterJSON();

	bool YReadTable();

private:
	bool YReadRow(const TSharedRef<FJsonObject>& InParsedTableRowObject, const int32 InRowIdx);

	bool YReadStruct(const TSharedRef<FJsonObject>& InParsedObject, UScriptStruct* InStruct, const FName InRowName, void* InStructData);

	bool YReadStructEntry(const TSharedRef<FJsonValue>& InParsedPropertyValue, const FName InRowName, const FString& InColumnName, const void* InRowData, FProperty* InProperty, void* InPropertyData);

	bool YReadContainerEntry(const TSharedRef<FJsonValue>& InParsedPropertyValue, const FName InRowName, const FString& InColumnName, const int32 InArrayEntryIndex, FProperty* InProperty, void* InPropertyData);

	UDataTable* DataTable;
	const FString& JSONData;
	TArray<FString>& ImportProblems;
};


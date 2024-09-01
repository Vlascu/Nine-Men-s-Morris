#include "FileManager.h"
#include "FileException.h"

std::string FileManager::GetDefaultInputFileName(EBoardType boardType, EBoardSize boardSize)
{
	std::string fileName = "../MoaraLogic/";

	switch (boardType)
	{
	case EBoardType::Normal:
		fileName += "Normal_";
		break;
	case EBoardType::Diagonals:
		fileName += "Diagonals_";
		break;
	default:
		throw FileException("Invalid board type");
	}
	switch (boardSize)
	{
	case EBoardSize::Normal:
		fileName += "Normal.txt";
		break;
	case EBoardSize::Big:
		fileName += "Big.txt";
		break;
	default:
		throw FileException("Invalid board size");
	}

	return fileName;
}

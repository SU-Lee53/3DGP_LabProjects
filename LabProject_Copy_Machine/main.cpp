#include <iostream>
#include <fstream>
#include <algorithm>
#include <filesystem>
#include <utility>	
#include <ranges>

std::pair<std::string, std::string> GetInput(std::string_view svSlnPath);
bool CreateDestDirectory(std::string_view svDestPath);
bool CopySrcToDest(std::string_view svSrcPath, std::string_view svDestPath);
bool ModifyCopiedFiles(std::string_view svSrcPath, std::string_view svDestPath);
bool ModifyRCFile(std::string_view svSrcPath, std::string_view svDestPath);

bool ChangeCopiedFilename(std::string_view svSrcPath, std::string_view svDestPath);
bool ModifyVCXPROJFile(std::string_view svSrcPath, std::string_view& svDestPath);
bool ReadYNInput();


int main()
{
	std::filesystem::path CurPath = std::filesystem::current_path();
	std::filesystem::path SolutionPath = CurPath.parent_path();

	auto [strSrcPath, strDestPath] = GetInput(SolutionPath.string());

	bool bResult = false;
	bResult = CreateDestDirectory(strDestPath);
	bResult = CopySrcToDest(strSrcPath, strDestPath);
	bResult = ModifyCopiedFiles(strSrcPath, strDestPath);

	if (bResult)
		return 0;
	else
		throw 20200401;

}


std::pair<std::string, std::string> GetInput(std::string_view svSlnPath)
{
	std::string strSrcPath, strDestPath;
	while (true) {
		std::cout << "Copy Source Project Name : ";
		std::cin >> strSrcPath;

		std::string strFinalSrcPath = std::string{ svSlnPath } + "\\" + strSrcPath;
		if (!std::filesystem::exists(std::filesystem::path{ strFinalSrcPath }))
		{
			std::cout << strFinalSrcPath << " : Copy Source Not Found" << '\n';
			strSrcPath.clear();
			continue;
		}
		std::cout << strFinalSrcPath << ": Copy Source Found" << '\n';


		std::cout << "Copy Destination Project Name : ";
		std::cin >> strDestPath;
		std::string strFinalDestPath = std::string{ svSlnPath } + "\\" + strDestPath;

		std::cout << strFinalDestPath << " : Is Correct? Y / N ";

		if (ReadYNInput()) {
			return { strFinalSrcPath, strFinalDestPath };
		}

	}

}

bool CreateDestDirectory(std::string_view svDestPath)
{
	std::filesystem::path dest{ svDestPath };
	if (std::filesystem::exists(dest)){
		while (true) {
			std::cout << dest.string() << " : already exist" << '\n';
			std::cout << "Proceed Anyway? (If proceed, existing directory's contents will be deleted) Y / N : ";
			if (ReadYNInput()) {
				// Delete existing directory
				std::filesystem::remove_all(dest);
				break;
			}
			else {
				return false;
			}
		}
	}

	// Create new directory
	if (!std::filesystem::create_directories(dest))
	{
		std::cout << "Failed to create directory" << '\n';
		return false;
	}

	std::cout << "Directory Successfully Created" << '\n';

}

bool CopySrcToDest(std::string_view svSrcPath, std::string_view svDestPath)
{
	std::filesystem::path src{ svSrcPath };
	std::filesystem::path dest{ svDestPath };
	std::filesystem::copy(src, dest, std::filesystem::copy_options::overwrite_existing);

	std::cout << "Directory files Successfully Copied" << '\n';
	return true;
}

bool ModifyCopiedFiles(std::string_view svSrcPath, std::string_view svDestPath)
{
	bool bResult = false;

	// 1. Change Filename
	bResult = ChangeCopiedFilename(svSrcPath, svDestPath);

	// 2. Modify vcxproj;
	bResult = ModifyVCXPROJFile(svSrcPath, svDestPath);

	// 3. Modify resource.rc
	bResult = ModifyRCFile(svSrcPath, svDestPath);
	return bResult;
}

bool ModifyRCFile(std::string_view svSrcPath, std::string_view svDestPath)
{
	return true;
}

bool ChangeCopiedFilename(std::string_view svSrcPath, std::string_view svDestPath)
{
	std::filesystem::path src{ svSrcPath };
	std::filesystem::path dest{ svDestPath };

	std::string SrcProjectName = src.stem().string();
	std::string DestProjectName = dest.stem().string();

	for (auto& DestContent : std::filesystem::directory_iterator{ dest }) {
		if (DestContent.path().filename().string().contains(SrcProjectName)) {
			std::string str = DestContent.path().string();
			std::cout << str << " Changed to : ";
			str.replace(str.find(SrcProjectName), SrcProjectName.size(), DestProjectName);
			std::cout << str << '\n';
			std::filesystem::rename(DestContent, std::filesystem::path{ str });
		}
	}

	return true;
}

bool ModifyVCXPROJFile(std::string_view svSrcPath, std::string_view& svDestPath)
{
	std::string projFileName{ std::string{ svDestPath } + "\\" + std::filesystem::path{ svDestPath }.stem().string() + ".vcxproj" };
	std::filesystem::path vcxprojFilePath{ projFileName };
	if (!std::filesystem::exists(vcxprojFilePath)) {
		std::cout << vcxprojFilePath.string() << " is not exist" << '\n';
		return false;
	}

	std::ifstream in{ projFileName };
	if (not in)
	{
		std::cerr << "Cannot open : " << projFileName << '\n';
		return false;
	}


	std::string findTarget = std::filesystem::path{ svSrcPath }.filename().string();
	std::string replaceTarget = std::filesystem::path{ svDestPath }.filename().string();
	std::string strFileString;
	std::copy(std::istreambuf_iterator{ in }, {}, std::back_inserter(strFileString));

	in.close();

	size_t targetLocation = strFileString.find(findTarget);
	while (targetLocation != std::string::npos)
	{
		strFileString.replace(targetLocation, findTarget.size(), replaceTarget);
		targetLocation = strFileString.find(findTarget);
	}

	std::ofstream out{ projFileName, std::ios::trunc };
	out << strFileString;

	return true;
}

bool ReadYNInput()
{
	while (true) {
		char c;
		std::cin >> c;
		if (c == 'y' || c == 'Y')
		{
			return true;
		}
		else if (c == 'n' || c == 'N')
		{
			return false;
		}
	}
}
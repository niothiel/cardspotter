// CSTest.cpp : Defines the entry point for the console application.
//

#include "QueryThread.h"
#include "CardDatabase.h"

//#pragma optimize("",off)
const char* cardPool = "priest of titania";
CardDatabase gDatabase;
Query query(gDatabase);

int main()
{
	std::cout << "Started test stuff" << std::endl;

	const int start = TimeNow();
	gDatabase.LoadFromTextFile("magic.db");
	std::cout << "Loaded database in " << (TimeNow() - start) << "ms" << std::endl;

	cv::setUseOptimized(false);
	cv::setNumThreads(1);
	query.myOkMatchScore = 280;
	query.bDebug = true;
	static bool videoTest = false;
	static bool clickTest = true;
	static bool singleTest = false;
	// Original values:
	// Min: 0.050000, Max: 0.200000
	printf("Pre: Min: %f, Max: %f\n", query.myMinCardHeightRelative, query.myMaxCardHeightRelative);

	if (singleTest)
	{
		float originalMin = query.myMinCardHeightRelative;
		float originalMax = query.myMaxCardHeightRelative;
		query.myMinCardHeightRelative = 0.5f;
		query.myMaxCardHeightRelative = 0.7f;
		query.TestFile("Regression/priest of titania.png");

		query.myMinCardHeightRelative = originalMin;
		query.myMaxCardHeightRelative = originalMax;

		// query.TestFile("Regression/tarfire.png");
		// std::cout << "Finished single test, exiting" << std::endl;
		// exit(29);
	}
	const int before = TimeNow();

// 	gDatabase.SetCardPool(cardPool, strlen(cardPool));

	int testTime = 0;
	int succeeded = 0;
	int tests = 0;
	Result result;
	std::vector<std::string> filenames;
	if (videoTest)
	{
		query.myMinCardHeightRelative = 0.5f;
		query.myMaxCardHeightRelative = 0.7f;

		getFileNames("Regression/Auto", filenames);

		for (const std::string& file : filenames)
		{
			if (file.find(".png") == -1)
			{
				continue;
			}

			cv::Mat* input;
			cv::Mat screen = cv::imread(file);
			input = &screen;

			cv::Mat resized;
			if (screen.rows != 720)
			{
				const float scale = float(screen.rows) / 720.f;
				cv::resize(screen, resized, cv::Size(int(float(screen.cols) / scale), 720));
				input = &resized;
			}

			std::cout << "|";
			if (query.AddScreenBGR(*input, testTime, result))
			{
				const Match& match = result.myMatch.myList[0];
				std::cout << std::endl << file << " [" << match.myScore[0] << ", " << match.myScore[1] << "][" << match.myDatabaseCard->mySetCode << "]: " << match.myDatabaseCard->myCardName << std::endl;
// 				cv::Mat inputImage = result.myMatch.myCard.mypotenatialRects[match.mypotentialRectIndex].myvariations[match.variant].myquery.myInputImage;
// 				cv::imwrite("Regression/DebugOutput/debugImage.png", inputImage);

				++succeeded;
			}
			testTime += 250;//simulate 4fps
			++tests;
// 			if (tests>32)
// 				break;
		}
	}

	if (clickTest)
	{
		getFileNames("Regression", filenames);

		for (const std::string& file : filenames)
		{
			// if (file.rfind("wildspeaker") == -1) {
			// 	continue;
			// }
			cv::Mat screen = cv::imread(file);
			// std::cout << "Rows " << screen.rows << " Cols " << screen.cols << std::endl;

			++tests;
			if (query.TestFile(file, false))
			{
				++succeeded;
			}

			// if(tests > 3) {
			// 	printf("WARNING: TODO: Breaking out of testing clicks early.\n");
			// 	break;
			// }
		}
	}

	std::cout << succeeded << "/" << tests << " in " << (TimeNow() - before) << "ms ";

	// std::ostringstream os_;
	// os_ << succeeded << "/" << tests << " in " << (TimeNow() - before) << "ms ";
	// OutputDebugString(os_.str().c_str());

	// int x;
	// std::cin >> x;
	return 0;
}


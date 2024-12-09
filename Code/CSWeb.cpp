#include <cstdio>
#include "CardDatabase.h"
#include "QueryThread.h"
#include "crow_all.h"
#include "base64.hpp"

CardDatabase gDatabase;

Result testImage(const cv::Mat &image)
{
    Result result;
    Query query(gDatabase);
    query.FindCardInRoiAndPrint(image, gDatabase.myCardLists, result);
    return result;
}

crow::json::wvalue marshalResult(const Result &result)
{
    crow::json::wvalue w;
    for (int i = 0; i < result.myMatch.myList.size(); i++)
    {
        auto &match = result.myMatch.myList[i];
        w["matches"][i]["card_id"] = match.myDatabaseCard->myCardId;
        w["matches"][i]["card_name"] = match.myDatabaseCard->myCardName;;
        w["matches"][i]["score"] = match.myScore[0];;
    }
    return w;
}

int main()
{
    crow::SimpleApp app;

	const int start = TimeNow();
	gDatabase.LoadFromTextFile("magic.db");
	std::cout << "Loaded database in " << (TimeNow() - start) << "ms" << std::endl;

    CROW_ROUTE(app, "/")([](){
        crow::json::wvalue w{{"hello", "world"}};
        return w;
    });
    CROW_ROUTE(app, "/detect").methods("POST"_method)([](const crow::request& req){
        auto x = crow::json::load(req.body);

        if (!x)
        {
            crow::json::wvalue w{{"error", "body is not a valid json"}};
            return crow::response(400, w);
        }
        if (!x.has("image_base64"))
        {
            crow::json::wvalue w{{"error", "image_base64 was not found in the body"}};
            return crow::response(400, w);
        }
        auto encodedImage = x["image_base64"].s();
        auto encodedImageStr = std::string(encodedImage);
        std::string image;
        try
        {
            image = base64::from_base64(encodedImageStr);
        }
        catch(const std::exception& e)
        {
            crow::json::wvalue w{{"error", "image_base64 is not a valid base64 encoded string"}};
            return crow::response(400, w);
        }

        std::vector<uchar> buffer(image.begin(), image.end());
        cv::Mat cvImage = cv::imdecode(buffer, cv::IMREAD_COLOR);
        if (cvImage.empty())
        {
            crow::json::wvalue w{{"error", "image could not be decoded"}};
            return crow::response(400, w);
        }

        auto result = testImage(cvImage);
        auto ret = marshalResult(result);

        // auto ret = crow::json::wvalue(x);
        return crow::response(200, ret);
    });

    const int port = 9000;
    app.port(port).run();
}

#pragma once

#define CURL_STATICLIB

#include <string>
#include <map>
#include <format>
#include "curl/curl.h"
#include <algorithm>

#pragma comment(lib, "curl/libcurl.lib")

using namespace std;

static bool bMcp = true;
static bool bUsingApi = false;
static bool bUsingWebhook = false;


static bool bDontNotifyMMS = false; // if true, the MMS will not be notified of the server going online!

// IF THIS IS TRUE IT WON'T NOTIFY MMS ANYWAY
static bool bStaffGameOnly = false; // if true, it will lock the game to only whitelisted usernames (and ips) from AdminNames and AdminIps


static string Region = "NA";
static string ServerName = "NAEONDefaultSolos"; // EDIT THIS
static int PlayersOnBus = 0;

map<string, int> VbucksToGive{};

#define API_URL "http://132.145.78.35:5555/"
#define MMS_URL "http://135.148.28.59/"
static string VPS_IP = "135.148.28.59"; // don't set this, you are weird
#define WEBHOOK_URL "https://discord.com/api/webhooks/1158856674954719334/TOrefOyKtNnFI43A21begGxCLjD4XllVyrEmjKpKqayv7PYO-hTmYBZc4Nt54HIXp_HU"

static size_t WriteCallback(char* contents, size_t size, size_t nmemb, void* RES)
{
	if (!contents || !RES)
		return 0;

	((std::string*)RES)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

static string ReplaceString(string str, string substr1, std::string substr2)
{
	for (size_t index = str.find(substr1, 0); index != std::string::npos && substr1.length(); index = str.find(substr1, index + substr2.length()))
		str.replace(index, substr1.length(), substr2);
	return str;
}

class DefaultAPI
{
public:
	DefaultAPI()
	{
		curl_global_init(CURL_GLOBAL_ALL);
		curl = curl_easy_init();

		if (!curl)
		{
			return;
		}

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curl_slist_append(NULL, "Content-Type: application/json"));
	}

	~DefaultAPI() {
		curl_global_cleanup();
		curl_easy_cleanup(curl);
	}

	FORCEINLINE bool PerformAction(const std::string& Endpoint, std::string* OutResponse = nullptr)
	{
		try
		{
			std::string URL = API_URL + Endpoint;

			auto out1 = curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());

			if (out1 != CURLE_OK)
			{
				printf("idk1\n");
				return false;
			}

			std::string TemporaryBuffer;
			//if (OutResponse)
			//{
			//	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
			//	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &TemporaryBuffer);
			//}

			auto out2 = curl_easy_perform(curl);
			cout << out2 << endl;
			if (out2 != CURLE_OK)
			{
				printf("idk2\n");
				return false;
			}
		}
		catch (...)
		{
			return false;
		}

		return true;
	}

	FORCEINLINE bool PerformActionMMS(const std::string& Endpoint, std::string* OutResponse = nullptr)
	{
		try
		{
			std::string URL = MMS_URL + Endpoint;

			auto out1 = curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());

			if (out1 != CURLE_OK)
			{
				printf("idk1\n");
				return false;
			}

			std::string TemporaryBuffer;
			//if (OutResponse)
			//{
			//	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
			//	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &TemporaryBuffer);
			//}

			auto out2 = curl_easy_perform(curl);
			cout << out2 << endl;
			if (out2 != CURLE_OK)
			{
				printf("idk2\n");
				return false;
			}
		}
		catch (...)
		{
			return false;
		}

		return true;
	}
protected:
	CURL* curl;
};

static size_t write_callback(char* ptr, size_t size, size_t nmenb, void* userdata) {
	((std::string*)userdata)->append(ptr, size * nmenb);
	return size * nmenb;
}

namespace EonMMSAPI {
		inline bool MarkServerOnlinev2(string REGION, string PlayerCap, string Port, string Session, string Playlist, string CustomCode) {

			std::string Endpoint = std::format("eon/gs/create/session/{}/{}/{}/{}/{}", REGION, VPS_IP, Port, Playlist, ServerName);

			std::string fullEndpoint = MMS_URL + Endpoint;

			curl_global_init(CURL_GLOBAL_ALL);
			CURL* curl = curl_easy_init();
			if (!curl) {
				fprintf(stderr, "Failed to initialize libcurl.\n");
				curl_global_cleanup();
			}

			//Set URL to API endpoint
			curl_easy_setopt(curl, CURLOPT_URL, fullEndpoint.c_str());


			// Set callback function for response body
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

			// Create a buffer to store the response body
			std::string response_body;

			// Set the buffer as the user-defined data for the callback function
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_body);

			// Perform HTTP request
			CURLcode res = curl_easy_perform(curl);

			if (res != CURLE_OK) {
				//fprintf(stderr, "Failed to perform HTTP request: %s\n", curl_easy_strerror(res));
				curl_easy_cleanup(curl);
				curl_global_cleanup();
				//UptimeWebHook.send_message("Failed to perform HTTP request for getting skin");
				return false;
			}

			// Check HTTP response code
			long response_code;
			curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
			if (response_code >= 200 && response_code < 300) {
				// HTTP request successful, check response body
				curl_easy_cleanup(curl);
				curl_global_cleanup();

				//UptimeWebHook.send_message("HTTP request successful for getting skin" + response_body);
				return true;

			}
			else {
				// HTTP request failed
				//fprintf(stderr, "HTTP request failed with status code %ld.\n", response_code);
				curl_easy_cleanup(curl);
				curl_global_cleanup();
				//UptimeWebHook.send_message("HTTP request failed with status code " + std::to_string(response_code) + " for getting skin");
				return false;
			}

		}

		inline bool SetServerStatus(string status) {

			std::string Endpoint = std::format("eon/gs/status/set/{}/{}", ServerName, status);

			std::string fullEndpoint = MMS_URL + Endpoint;

			curl_global_init(CURL_GLOBAL_ALL);
			CURL* curl = curl_easy_init();
			if (!curl) {
				fprintf(stderr, "Failed to initialize libcurl.\n");
				curl_global_cleanup();
			}

			//Set URL to API endpoint
			curl_easy_setopt(curl, CURLOPT_URL, fullEndpoint.c_str());


			// Set callback function for response body
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

			// Create a buffer to store the response body
			std::string response_body;

			// Set the buffer as the user-defined data for the callback function
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_body);

			// Perform HTTP request
			CURLcode res = curl_easy_perform(curl);

			if (res != CURLE_OK) {
				//fprintf(stderr, "Failed to perform HTTP request: %s\n", curl_easy_strerror(res));
				curl_easy_cleanup(curl);
				curl_global_cleanup();
				//UptimeWebHook.send_message("Failed to perform HTTP request for getting skin");
				return false;
			}

			// Check HTTP response code
			long response_code;
			curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
			if (response_code >= 200 && response_code < 300) {
				// HTTP request successful, check response body
				curl_easy_cleanup(curl);
				curl_global_cleanup();

				//UptimeWebHook.send_message("HTTP request successful for getting skin" + response_body);
				return true;

			}
			else {
				// HTTP request failed
				//fprintf(stderr, "HTTP request failed with status code %ld.\n", response_code);
				curl_easy_cleanup(curl);
				curl_global_cleanup();
				//UptimeWebHook.send_message("HTTP request failed with status code " + std::to_string(response_code) + " for getting skin");
				return false;
			}

		}

		
}

	namespace Discord {
		class DiscordWebhook {
		public:
			DiscordWebhook(const char* webhook) {
				curl_global_init(CURL_GLOBAL_ALL);
				curl = curl_easy_init();

				if (curl) {
					curl_easy_setopt(curl, CURLOPT_URL, webhook);
					curl_slist* headers = curl_slist_append(NULL, "Content-Type: application/json");
					curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
				}
				else {
					printf("Failed To Send! Will Return as a NULL POINTER");
				}
			}

			~DiscordWebhook() {
				curl_global_cleanup();
				curl_easy_cleanup(curl);
			}

			inline bool send_message(const std::string& message) {
				if (!bUsingWebhook)
					return false;
				std::string json = "{\"content\": \"" + message + +"\"}";
				curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.c_str());

				return curl_easy_perform(curl) == CURLE_OK;
			}
		private:
			CURL* curl;
		};


		static DiscordWebhook Servers(WEBHOOK_URL);

	}

	namespace EonApi
	{
		static DefaultAPI* StaticAPI = nullptr;
		void SetupAPI()
		{
			if (!bUsingApi)
				return;

			StaticAPI = new DefaultAPI();
		}

		void MarkServerOnline(string REGION, string PlayerCap, string Port, string Session, string Playlist, string CustomCode)
		{
			cout << Discord::Servers.send_message(Region + " is now up!") << endl;
			if (StaticAPI)
			{
				std::string Endpoint = std::format("fortnite/api/eon/add/{}/{}/{}/{}/{}/{}", Session, REGION, Port, Playlist, PlayerCap, CustomCode);
				if (!StaticAPI->PerformAction(Endpoint))
				{
					printf("failed\n");
				}
			}
		}

		// kill yourself you stupid crappy visual studio wannabe.
		void LockGameSession(string Region, string Name)
		{
			Discord::Servers.send_message(Region + " match started!");
			if (StaticAPI)
			{
				std::string Enp = std::format("fortnite/api/eon/lock/{}/{}", Region, Name);
				if (!StaticAPI->PerformAction(Enp))
				{
					printf("failed\n");
				}
			}
		}

		void RemoveSession(string Region, string Name)
		{
			Discord::Servers.send_message(Region + " server is restarting...");
			if (StaticAPI)
			{
				std::string Endp = std::format("fortnite/api/eon/remove/{}/{}", Region, Name);
				if (!StaticAPI->PerformAction(Endp))
				{
					printf("failed\n");
				}
			}
		}

		void AddVbucksOK(string Amount, string PlaylistName, string WinOrNot, string AccountID)
		{
			if (StaticAPI)
			{
				cout << AccountID << endl;
				std::string Endp = std::format("gameserver/add/{}/{}/{}/{}", Amount, PlaylistName, WinOrNot, AccountID);
				string ret = "idajasdkjfa";
				if (!StaticAPI->PerformAction(Endp, &ret))
				{
					printf("failed\n");
				}
				cout << ret << endl;
			}
		}

		void GiveVbucks(string Amount, string PlaylistName, string WinOrNot, string AccountID)
		{
			if (PlayersOnBus > 1)
				AddVbucksOK(Amount, PlaylistName, WinOrNot, AccountID);
			//new thread(AddVbucksOK, Amount, PlaylistName, WinOrNot, AccountID);
		}

		void BanPlayer(string AccountID)
		{
			if (StaticAPI)
			{
				std::string Endp = std::format("gs/ban/{}", AccountID);
				if (!StaticAPI->PerformAction(Endp))
				{
					printf("failed\n");
				}
			}
		}

		bool IsBanned(string AccountID)
		{
			if (!StaticAPI)
				return false;

			std::string Endp = std::format("gs/check/{}", AccountID);
			string OutResponse = "true";
			if (!StaticAPI->PerformAction(Endp, &OutResponse))
			{
				printf("failed\n");
				return true;
			}

			return OutResponse == "true";
		}
	}

	//hosting@gmail.com
	//3d7972759df74270bac6b09f992dd7b1
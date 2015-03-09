/*
	Copyright (c) 2014 - 2015 Rafa³ "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafa³ Grasman <grasmanek94@gmail.com>
		* Development Start Month-Year: October-2014

	________________________________________________________________
	Purpose of this file

	________________________________________________________________	
	Notes

	________________________________________________________________
	Dependencies

	________________________________________________________________
	Project Contributors
		Tom 'IceCube' Hewlett
		Mariusz 'C2A1' K.
		Mateusz 'eider' Cichoñ
*/
#include "GameMode.hxx"
/*
std::string Base64Encode(std::string input)
{
	std::stringstream os;
	typedef
		boost::archive::iterators::base64_from_binary<    // convert binary values to base64 characters
			boost::archive::iterators::transform_width<   // retrieve 6 bit integers from a sequence of 8 bit bytes
				const char *,
				6,
				8
			>
		>
		base64_text; // compose all the above operations in to a new iterator

	std::copy(
		base64_text(input.c_str()),
		base64_text(input.c_str() + input.size()),
		boost::archive::iterators::ostream_iterator<char>(os)
		);
	return os.str();
}*/

void SendMail(int index, std::string to, std::string message_id, std::string send_data, std::string return_data, std::string language)
{
	sampgdk_HTTP(index, HTTP_POST, "exe24.info/svl/mailerv2.php", ("to=" + to + "&msgid=" + message_id + send_data + "&rd=" + return_data + "&lang=" + language).c_str());
}

class EmailProcessor : public Extension::Base
{
public:
	void OnHTTPResponse(int index, int response_code, std::string data) override
	{ 
		if (response_code == 200 && index == 0x7733)
		{
			ParseInput parser("dp", data, ':');
			if (parser.Good() == 2)
			{
				long emailresult = parser.Get<long>(0);
				ParsePlayer player = parser.Get<ParsePlayer>(1);
				if (player.playerid != INVALID_PLAYER_ID)
				{
					if (emailresult != 2)
					{
						fixSendClientMessage(player.playerid, 0xFF0000FF, L_email_send_error);
					}
					else
					{
						fixSendClientMessage(player.playerid, 0x00FF00FF, L_email_send_success);
					}
				}
			}
		}
	}
} _EmailProcessor;

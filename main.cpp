#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <thread>
#include <limits>
#include <sstream>

typedef std::string str;
typedef std::vector<int> int_vec;
typedef std::pair<int_vec, int_vec> int_vec_pair;
typedef std::stringstream str_stream;

str load_input(const str& prompt);
int_vec encode(const char& letter);
int_vec_pair encode_all(const str& data);
str build_str(const int_vec& chars);
str interpret_key(const int_vec& nums);
int_vec reverse_key(const str& raw_key);
int_vec reverse_code(const str& raw_code);
str decode(const int_vec& code, const int_vec& key);
str lower(const str& original);

enum Options
{
	decode_op = 'd',
	encode_op = 'e',
};

int main(void)
{
	srand(time(0));
	str prompt0 = "\x1b[1;33m[INPUT]:\x1b[0m \x1b[1;32mPlease, enter D to decode or E to encode.\x1b[0m ";
	str prompt1 = "\x1b[1;33m[INPUT]:\x1b[0m \x1b[1;32mPlease, enter a string to decode.\x1b[0m ";
	str prompt2 = "\x1b[1;33m[INPUT]:\x1b[0m \x1b[1;32mPlease, enter a string to encode.\x1b[0m ";
	str prompt3 = "\x1b[1;33m[INPUT]:\x1b[0m \x1b[1;32mPlease, enter a key to decode the string with.\x1b[0m ";
	while (true)
	{
		str option = load_input(prompt0);
		option = lower(option);

		if (option[0] == Options::decode_op)
		{
			str raw_code = load_input(prompt1);
			str raw_key = load_input(prompt3);
			int_vec shifts = reverse_key(raw_key);
			int_vec chars = reverse_code(raw_code);
			str decoded_str = decode(chars, shifts);
			std::cout << "\x1b[1;33m[OUTPUT]:\x1b[0m \x1b[1;32m" << decoded_str << "\x1b[0m" << '\n' << '\n';
		}
		else
		{
			str to_encode = load_input(prompt2);
			int_vec_pair encoded = encode_all(to_encode);
			int_vec chars = encoded.first;
			int_vec shifts = encoded.second;
			str encoded_str = build_str(chars);
			str built_key = interpret_key(shifts);
			std::cout << "\x1b[1;33m[OUTPUT]:\x1b[0m \x1b[1;32m" << built_key << "\x1b[0m" << '\n';
			std::cout << "\x1b[1;33m[OUTPUT]:\x1b[0m \x1b[1;32m" << encoded_str << "\x1b[0m" << '\n' << '\n';
		}
	}

	std::this_thread::sleep_for(std::chrono::seconds(15));
	return 0;
}

str load_input(const str& prompt)
{
	str input;
	
	do 
	{
		std::cout << prompt;
		std::getline(std::cin >> std::ws, input);

		if (std::cin.fail() || input.empty())
		{
			std::cout << "\x1b[1;31m[ERROR]: \x1b[31mInvalid input! Please insert a valid entry.\x1b[0m";
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}

	} while (input.empty() || std::cin.fail());

	return input;
}

int_vec encode(const char& letter)
{
	int random = std::rand() % 26;
	int charCode = (int)letter;

	if (charCode >= 65 && charCode <= 90)
	{
		charCode = ((charCode - 65 + random) % 26) + 65;
	}
	else if (charCode >= 97 && charCode <= 122)
	{
		charCode = ((charCode - 97 + random) % 26) + 97;
	}
	
	int_vec encoded = {charCode, random};
	return encoded;
}

int_vec_pair encode_all(const str& data)
{
	int_vec key;
	int_vec chars;

	for (size_t i = 0; i < data.size(); i++)
	{
		int_vec encoded = encode(data[i]);
		chars.push_back(encoded[0]);
		key.push_back(encoded[1]);
	}

	return { chars, key };
}

str build_str(const int_vec& chars)
{
	str new_str;
	
	for (size_t i = 0; i < chars.size(); i++)
	{
		new_str.push_back((char)chars[i]);
	}

	return new_str;
}

str interpret_key(const int_vec& nums)
{
	str key;

	for (size_t i = 0; i < nums.size(); i++)
	{
		if (i != 0) key += ',';
		key += std::to_string(nums[i]);
	}

	return key;
}

int_vec reverse_code(const str& raw_code)
{
	int_vec output;
	
	for (size_t i = 0; i < raw_code.size(); i++)
	{
		int char_code = (int)raw_code[i];
		output.push_back(char_code);
	}
	
	return output;
}

int_vec reverse_key(const str& raw_key)
{
	int_vec shifts;
	str temp;
	str_stream ss (raw_key);
	
	while (std::getline(ss, temp, ','))
	{
		shifts.push_back(std::stoi(temp));
	}

	return shifts;
}

str decode(const int_vec& code, const int_vec& key)
{
	str output;
	if (code.size() != key.size()) return output;

	for (size_t i = 0; i < code.size(); i++)
	{
		int ascii_char = code[i];
		int ascii_shift = key[i];
		char original_char;

		if (ascii_char >= 65 && ascii_char <= 90)
		{
			original_char = (char)((ascii_char - 65 - ascii_shift + 26) % 26) + 65;
		}
		else if (ascii_char >= 97 && ascii_char <= 122)
		{
			original_char = (char)((ascii_char - 97 - ascii_shift + 26) % 26) + 97;
		}
		else
		{
			original_char = (char)(ascii_char);
		}

		output.push_back(original_char);
	}

	return output;
}

str lower(const str& original)
{
	str result = original;
	std::transform(result.begin(), result.end(), result.begin(), ::tolower);
	return result;
}

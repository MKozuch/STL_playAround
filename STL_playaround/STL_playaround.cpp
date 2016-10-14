// author: MKozuch


#include "stdafx.h"
#include <stdio.h> // gwarantuje dostep do symboli w globalnym nejpspejsie
// #include <cstdio.h> // gwarantuje dostep do symbolu w nejmspejsie std

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <iterator>

#include <locale>
#include <codecvt>

class myInt {

private:
	int val;

public:
	myInt(int v = 0):val(v){
	}

};

bool is_char_alpha_or_space(char c) {
	std::locale loc = std::locale("Polish");
	bool result1 = std::isalpha(c, loc);
	bool result2 = std::isspace(c, loc);
	return result1 || result2;
}

std::string strip_puntuation(std::string sourcestr) {
	std::string deststring;
	deststring.reserve(sourcestr.size());
	std::copy_if(sourcestr.begin(), sourcestr.end(), back_inserter(deststring), is_char_alpha_or_space);
	return deststring;
}

std::vector<std::string> tokenize_string(std::string sourcestr) {
	std::vector<std::string> output_vect;
	std::istringstream ss(sourcestr);
	
	while (ss) {
		std::string str;
		std::getline(ss, str, ' ');
		if(!str.empty()) output_vect.push_back(str);
	}
	return output_vect;
}

std::string to_lowercase(std::string sourcestr) {
	std::string deststring;
	std::transform(sourcestr.begin(), sourcestr.end(), std::back_inserter(deststring), tolower);
	return deststring;
}

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "Polish");

	std::string fname;
	if (argc > 1) fname = argv[1];
	else fname = "pan-tadeusz2.txt";

	std::ifstream fhandle;
	fhandle.open(fname);

	if (!fhandle.is_open()) {
		printf("Something went wrong");
		return EXIT_FAILURE;
	}

	std::string line;
	std::string stopline("Ksiêga druga");
	typedef std::vector<std::string> stringvector;
	stringvector allwordvector;
	stringvector tempvector;

	// populate and sort vector containing all words 
	while ( !(fhandle.eof())) {
		std::getline(fhandle, line);
		if (line == stopline) break;
		line = strip_puntuation(line);
		line = to_lowercase(line);
		tempvector = tokenize_string(line);
		std::copy(tempvector.begin(), tempvector.end(), std::back_inserter(allwordvector));
	}
	std::sort(allwordvector.begin(), allwordvector.end());

	// remove repeated words by converting vector to set
	typedef std::set<std::string> stringsettypedef;
	stringsettypedef uniquewords(allwordvector.begin(), allwordvector.end());

	// create map containig the word and the number of its occurences
	typedef std::map<std::string, unsigned int> wordcountmaptypedef;
	typedef std::pair<std::string, unsigned int> record;
	wordcountmaptypedef wordcount;
	stringsettypedef::iterator worditerator;	
	unsigned int count;
	for (worditerator = uniquewords.begin(); worditerator != uniquewords.end(); ++worditerator) {
		count = std::count(allwordvector.begin(), allwordvector.end(), *worditerator);
		wordcount.insert(record(*worditerator, count));
	}
	
	// swap map  and convert occurences to relative frequency using LAMBDA o.O
	unsigned int totalwordscount = allwordvector.size();
	wordcountmaptypedef::iterator maxoccurence = std::max_element(wordcount.begin(), wordcount.end(),
		[](const record &left, const record &right) {return left.second < right.second; });
	unsigned int maxoccurencenumber = maxoccurence->second;

	std::multimap<double, std::string> wordfrequencymap;
	std::map<std::string, unsigned int>::iterator mapiterator;
	typedef std::pair<double, std::string> wordfrequencyrecord;
	for (mapiterator = wordcount.begin(); mapiterator != wordcount.end(); ++mapiterator) {
		wordfrequencymap.insert(wordfrequencyrecord((double)mapiterator->second / maxoccurencenumber, mapiterator->first ));
	}

	// print top 10 freq
	std::multimap<double, std::string>::iterator freqmapiter = wordfrequencymap.end();
	for (int i = 0; i < 25; ++i) {
		freqmapiter--;
		printf("%E - %s\n", freqmapiter->first, freqmapiter->second.c_str());
	}

    return EXIT_SUCCESS;
}


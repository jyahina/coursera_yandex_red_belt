#pragma once
#include "starts.h"

void Stats::AddMethod(string_view method)
{
	(methods.find(method) != methods.end())
		? ++methods[method]
		: ++methods["UNKNOWN"];
}

void Stats::AddUri(string_view uri)
{
	(uris.find(uri) != uris.end())
		? ++uris[uri]
		: ++uris["unknown"];
}

const map<string_view, int>& Stats::GetMethodStats() const
{
	return methods;
}

const map<string_view, int>& Stats::GetUriStats() const
{
	return uris;
}

void RemovePrefix(string_view& line, size_t pos = 0) 
{
	line.remove_prefix(pos);
	line.remove_prefix(std::min(line.find_first_not_of(" "), line.size()));
}

string_view GetWord(string_view& line) 
{
	RemovePrefix(line);
	auto pos = line.find(' ');
	auto result = line.substr(0, pos);

	RemovePrefix(line, ((pos != line.npos) ? pos : 0));
	return result;
}

HttpRequest ParseRequest(string_view line) 
{
	HttpRequest request;

	request.method = GetWord(line);
	request.uri = GetWord(line);
	request.protocol = GetWord(line);

	return request;
}

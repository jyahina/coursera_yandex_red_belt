#include "search_server.h"
#include "iterator_range.h"
#include "parse.h"
#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>
#include <utility>
#include <functional>

vector<string> SplitIntoWords(const string& line)
{
	istringstream words_input(line);
	return { istream_iterator<string>(words_input), istream_iterator<string>() };
}

void AddQueriesStreamByThread(istream& query_input, ostream& search_results_output, Synchronized<InvertedIndex>& protected_index)
{
	//auto index = protected_index.GetAccess();

	vector<pair<int, size_t>> docid_count_2(protected_index.GetAccess().ref_to_value.GetDocument().size());

	for (string current_query; getline(query_input, current_query); )
	{
		docid_count_2.resize(protected_index.GetAccess().ref_to_value.GetDocument().size());
		docid_count_2.assign(docid_count_2.size(), { -1, 0 });
		const auto words = SplitIntoWords(current_query);

		for (const auto& word : words)
		{
			for (const size_t docid : protected_index.GetAccess().ref_to_value.Lookup(word))
			{
				docid_count_2[docid].first = static_cast<int>(docid);
				docid_count_2[docid].second++;
			}
		}

		partial_sort(docid_count_2.begin(), Head(docid_count_2, 5).end(), docid_count_2.end(),
			[](pair<int, size_t>& lhs, pair<int, size_t>& rhs)
			{ return pair(lhs.second, -lhs.first) > pair(rhs.second, -rhs.first); });

		search_results_output << current_query << ':';
		for (auto [docid, hitcount] : Head(docid_count_2, 5)) {
			if (docid == -1) continue;

			search_results_output << " {"
				<< "docid: " << docid << ", "
				<< "hitcount: " << hitcount << '}';
		}

		search_results_output << '\n';
	}
}

void UpdateDocumentBaseByThread(istream& document_input, Synchronized<InvertedIndex>& protected_index)
{
	InvertedIndex new_index;

	for (string current_document; getline(document_input, current_document); ) {
		new_index.Add(move(current_document));
	}
	protected_index.GetAccess().ref_to_value = move(new_index);
}
 
SearchServer::SearchServer(istream& document_input) {
	UpdateDocumentBaseByThread(document_input, index);
}

void SearchServer::UpdateDocumentBase(istream& document_input)
{
	UpdateDocumentBaseByThread(document_input, index);
	//tasks.push_back(async(UpdateDocumentBaseByThread, ref(document_input), ref(index)));
}

void SearchServer::AddQueriesStream(istream& query_input, ostream& search_results_output)
{
	AddQueriesStreamByThread(query_input, search_results_output, index);
	//tasks.push_back(async(AddQueriesStreamByThread, ref(query_input), ref(search_results_output), ref(index)));
}

void InvertedIndex::Add(const string& document) {
	docs.push_back(document);

	const size_t docid = docs.size() - 1;
	for (const auto& word : SplitIntoWords(document)) {
		index[word].push_back(docid);
	}
}

list<size_t> InvertedIndex::Lookup(const string& word) const
{
	if (auto it = index.find(word); it != index.end())
	{
		return it->second;
	}
	else {
		return {};
	}
}

//list<size_t> InvertedIndex::Lookup(string_view word) const
//{
//	if (auto it = index.find(word); it != index.end())
//	{
//		return it->second;
//	}
//	else {
//		return {};
//	}
//}

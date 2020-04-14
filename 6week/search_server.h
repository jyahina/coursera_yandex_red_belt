#pragma once

#include <istream>
#include <ostream>
#include <set>
#include <list>
#include <vector>
#include <map>
#include <string>
#include <future>
#include "synchronized.h"
#include "profile.h"
using namespace std;

class InvertedIndex {
public:
  void Add(const string& document);
  list<size_t> Lookup(const string& word) const;
  //list<size_t> Lookup(string_view word) const;

  const string& GetDocument(size_t id) const 
  {
    return docs[id];
  }

  const vector<string>& GetDocument() const
  {
      return docs;
  }

private:
 map<string, list<size_t>> index;
  //map<string_view, list<size_t>> index;
  vector<string> docs;
};

class SearchServer {
public:
  SearchServer() = default;
  explicit SearchServer(istream& document_input);
  void UpdateDocumentBase(istream& document_input);
  void AddQueriesStream(istream& query_input, ostream& search_results_output);

private:
  Synchronized<InvertedIndex> index;
  vector<future<void>> tasks;
  
};

#include "inplace_string/inplace_string.h"
#include "static_any/any.hpp"

#include <boost/container/static_vector.hpp>

#include <iostream>
#include <vector>
#include <experimental/any>

namespace InPlace
{
using MetadataTag = inplace_string<15>;
using MetadataValue = static_any<16>;

using Metadata = std::pair<MetadataTag, MetadataValue>;

struct MetadataTree : public boost::container::static_vector<Metadata, 8>
{
	template <typename StringT, typename ValueT>
	void Add(StringT&& str, ValueT&& value)
	{
		this->emplace_back(std::forward<StringT>(str), std::forward<ValueT>(value));
	}
};
}

using MetadataTag = std::string;
using MetadataValue = std::experimental::any;

using Metadata = std::pair<MetadataTag, MetadataValue>;

struct MetadataTree : public std::vector<Metadata>
{
	template <typename StringT, typename ValueT>
	void Add(StringT&& str, ValueT&& value)
	{
		this->emplace_back(std::forward<StringT>(str), std::forward<ValueT>(value));
	}
};

template <typename TreeT>
TreeT GetTree(int i, double d, bool b)
{
	TreeT tree;
	tree.Add("metadata1", i);
	tree.Add("foobaz", d);
	tree.Add("foobuz", b);
	return tree;
}

MetadataTree GetTree(int i, double d, bool b) { return GetTree<MetadataTree>(i, d, b); }
InPlace::MetadataTree GetInPlaceTree(int i, double d, bool b) { return GetTree<InPlace::MetadataTree>(i, d, b); }


int main ()
{
	auto tree = GetTree(1, 2.0, true);
	auto tree2 = GetInPlaceTree(1, 2.0, true);
	std::cout << tree.size() << std::endl;
	std::cout << tree2.size() << std::endl;
}

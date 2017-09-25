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

struct MetadataTree
{
	template <typename StringT, typename ValueT>
	MetadataTree& Add(StringT&& str, ValueT&& value)
	{
		_metadata.emplace_back(std::forward<StringT>(str), std::forward<ValueT>(value));
		return *this;
	}

	boost::container::static_vector<Metadata, 8> _metadata;
};
}

using MetadataTag = std::string;
using MetadataValue = std::experimental::any;

using Metadata = std::pair<MetadataTag, MetadataValue>;

struct MetadataTree
{
	template <typename StringT, typename ValueT>
	MetadataTree& Add(StringT&& str, ValueT&& value)
	{
		_metadata.emplace_back(std::forward<StringT>(str), std::forward<ValueT>(value));
		return *this;
	}

	std::vector<Metadata> _metadata;
};

template <typename TreeT>
TreeT GetTree(int i, double d, bool b)
{
	TreeT tree;
	tree.Add("metadata1", i).Add("metadata2", d).Add("metadata3", b);
	return tree;
}

int BenchmarkGetTree()
{
	int elements = 0; // just to prevent compiler optimizations

	for (int i = 0; i < 1000000; ++i)
	{
		auto tree = GetTree<MetadataTree>(elements, 2.0, true);
		elements += tree._metadata.size() + tree._metadata[0].second.empty();
	}

	return elements;
}


int BenchmarkGetInPlaceTree()
{
	int elements = 0;
	for (int i = 0; i < 1000000; ++i)
	{
		auto tree = GetTree<InPlace::MetadataTree>(elements, 2.0, true);
		elements += tree._metadata.size() + tree._metadata[0].second.empty();
	}
	return elements;
}

int main (int argc, char** argv)
{
	if (argc != 2)
	{
		std::cerr << "usage: " << argv[0] << "<tree|inplace_tree>";
		return 1;
	}

	if (argv[1] == std::string("tree"))
	{
		int unused = BenchmarkGetInPlaceTree();
		std::cout << "inplace:" << unused << std::endl;
	}
	else
	{
		int unused = BenchmarkGetTree();
		std::cout << "notinplace:" << unused << std::endl;
	}
}

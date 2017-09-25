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
	void Add(StringT&& str, ValueT&& value)
	{
		_metadata.emplace_back(std::forward<StringT>(str), std::forward<ValueT>(value));
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
	void Add(StringT&& str, ValueT&& value)
	{
		_metadata.emplace_back(std::forward<StringT>(str), std::forward<ValueT>(value));
	}

	std::vector<Metadata> _metadata;
};

template <typename TreeT>
TreeT GetTree(int i, double d, bool b)
{
	TreeT tree;
	tree.Add("metadata1", i);
	tree.Add("metadata2", d);
	tree.Add("metadata3", b);
	return tree;
}

int BenchmarkGetTree()
{
	int elements = 0;
	for (int i = 0; i < 1000000; ++i)
	{
		auto tree = GetTree<MetadataTree>(1, 2.0, true);
		elements += tree._metadata.size() + std::experimental::any_cast<int>(tree._metadata[0].second);
	}
	return elements;
}


int BenchmarkGetInPlaceTree()
{
	int elements = 0;
	for (int i = 0; i < 1000000; ++i)
	{
		auto tree = GetTree<InPlace::MetadataTree>(1, 2.0, true);
		elements += tree._metadata.size() + any_cast<int>(tree._metadata[0].second);
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

	const std::string argv1(argv[1]);

	if (argv1 == "tree")
	{
		int unused = BenchmarkGetTree();
		std::cout << "inplace:" << unused << std::endl;
	}
	else if (argv1 == "inplace_tree")
	{
		int unused = BenchmarkGetInPlaceTree();
		std::cout << "notinplace:" << unused << std::endl;
	}
}

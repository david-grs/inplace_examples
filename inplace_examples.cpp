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

MetadataTree GetTree(int i, double d, bool b) { return GetTree<MetadataTree>(i, d, b); }
InPlace::MetadataTree GetInPlaceTree(int i, double d, bool b) { return GetTree<InPlace::MetadataTree>(i, d, b); }

int main (int argc, char** argv)
{
	if (argc != 2)
	{
		std::cerr << "usage: " << argv[0] << "<inplace|not>";
		return 1;
	}

	if (argv[1] == std::string("inplace"))
	{
		int unused = 0;

		for (int i = 0; i < 1000000; ++i)
		{
			auto tree = GetInPlaceTree(unused, 2.0, true);
			unused += tree._metadata.size();
		}

		std::cout << "inplace" << unused << std::endl;
	}
	else
	{
		int unused = 0;

		for (int i = 0; i < 1000000; ++i)
		{
			auto tree = GetTree(unused, 2.0, true);
			unused += tree._metadata.size();
		}

		std::cout << "notinplace" << unused << std::endl;
	}
}

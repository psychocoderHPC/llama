#pragma once

#include "../Types.hpp"

namespace llama
{

template<
	typename __UserDomain,
	typename __DateDomain
>
struct MappingInterface
{
	using UserDomain = __UserDomain;
	using DateDomain = __DateDomain;
	MappingInterface(const UserDomain);
	static constexpr size_t blobCount = 0;
	inline size_t getBlobSize(const size_t blobNr) const;
	template <typename DateDomainCoord>
	inline BlobAdress getBlobAdress(const UserDomain coord,const UserDomain size) const;
};

} //namespace llama
#include "pch.h"

#include <iostream>

#include "..\recs\Entity.h"
TEST(Entity, null) {

	using traits = recs::entity_traits<std::underlying_type_t<recs::Entity>>;

	

	// Comparision operators
	// Check null == null -> true
	ASSERT_TRUE(recs::null == recs::null);
	// Check null != null -> false
	ASSERT_FALSE(recs::null != recs::null);
	// Check null == Entity -> false
	ASSERT_FALSE(recs::null == recs::Entity{});
	// Check null != Entity -> true
	ASSERT_TRUE(recs::null != recs::Entity{});
	// Check Entity != null -> true
	ASSERT_TRUE(recs::Entity{} != recs::null);
	// Check Entity == null -> false
	ASSERT_FALSE(recs::Entity{} == recs::null);

	// Get null cast as an entity using the conversion operator
	const auto null_entity = static_cast<recs::Entity>(recs::null);

	std::cout << to_integral(null_entity) << std::endl;

	// Conversion operator 
	// Check null == Entity{entity_mask} -> true
	ASSERT_TRUE(recs::null == recs::Entity{ traits::entity_mask } );
	// Check Entity(null) == null -> true
	ASSERT_TRUE(null_entity == recs::null);
	// Check Entity(null) != null -> false
	ASSERT_FALSE(null_entity != recs::null);
}
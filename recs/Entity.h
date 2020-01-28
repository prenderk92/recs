#pragma once

#include<type_traits>

/*------------------------------------------------------------------------------
* In this ECS framework Entities are used only as identifiers and thus only   
* contain an Entity ID and a version number. The version number will allow us to
* make sure the entity a system may have reference to hasn't been altered 
* without the system knowing. Ideally, we would like to cram this Entity class
* into a 16,32, or 64 bit space depending on the needs of the user. Using
* template overloads, we can force the underlying type of an Entity as seen in
* the entity_traits struct. The details on how to read entity_traits definitions
* are as follows:
*	
*			Entity (16-bit): 0 0 0 0 0 0 0 0 0 0 0 1 | 0 0 0 1
*								  				  ID | Version
*						vmask_shift = 12 (version starts 12 bits in)
*						entity_mask = 0xFFF (12-bits worth of data for ID),
*						version_mask = 0xF (4-bits of data for Version)
*	
* The entity_mask and version_mask are used to pull out the values by bit-wise 
* comparision, and vmask_shift describes the number of bits our version number 
* starts at. Additionally, we have to define the types we will cast our Entity 
* ID to, the type for Entity Version, and the type used for indexing and for
* the difference of iterators over arrays of this type.
*
* Additionally, to accomodate the needs of our SparseSet, we need to define a
* null Entity class. To make the null Entity class, we simply need to define
* comparision operators between null types and entity types as well as a
* conversion operator so that the null Entity can be properly cast to an Entity 
* type.
*-----------------------------------------------------------------------------*/

// TODO: Move this kind of stuff into a separate config file
// Define a macro for the base type of all Entity ID's in our framework
#ifndef RECS_ENTITY_ID_TYPE 
#	include <cstdint>
#	define RECS_ENTITY_ID_TYPE std::uint16_t
#endif

namespace recs {

	// An enum class that acts as an alias for our underlying Entity ID type.
	// Hopefully, this makes it clear that an Entity is really just a 16-bit
	// unsigned integer used as an identifier.
	// TODO: Wrap the underlying entity ID type in a macro and move it to a
	//		config file
	enum class Entity : std::uint16_t {}; 

	// This function takes an Entity and converts it to it's underlying
	// integral type. The function is constexpr so it should not incurr any
	// runtime cost.
	constexpr auto to_integral(const Entity ent) noexcept
	{
		return static_cast<std::underlying_type_t<Entity>>(ent);
	}

	// Forward declare entity_traits template
	template<typename>
	struct entity_traits;

	// The following code is used to describe the traits of a 16-bit entity id.
	// For details on the thoughts behind this, see the description at the start
	// of the file.
	template<>
	struct entity_traits<std::uint16_t>
	{
		// This alias is the underlying size type of the entity
		using entity_type = std::uint16_t;

		// This alias describes the underlying size type of the version
		using version_type = std::uint8_t;

		// The alias describing the index type
		using index_type = std::uint32_t;

		// The bit-mask used to get the Entity id
		static constexpr std::uint16_t entity_mask = 0xFFF;

		// The bit-mask used to get the version
		static constexpr std::uint16_t version_mask = 0xF;

		// The number of bits to shift before applying the version mask
		static constexpr auto vmask_shift = 12;
	};

	// We will want to have null Entities to populate our SparseArray with. To
	// implement a null entity, we need to define operators for comparision and
	// a conversion operator from null to Entity.
	//
	// We can put this class into an internal name space and write an alias to
	// this class using a constexpr value (because null contains no data).
	
	namespace internal {

		class null {
			// Define an alias to access the values in entity_traits using the
			// underlying type of Entity
			template<typename Entity>
			using traits = entity_traits<std::underlying_type_t<Entity>>;

		public:
			// Define conversion operator from null to Entity implicitly. We want to
			// use a conversion operator so that null can be returned an an Entity
			template<typename Entity>
			constexpr operator Entity() const noexcept {
				// If our null is converted to an Entity, return an entity with an
				// ID set to the max ID.
				return Entity{ traits<Entity>::entity_mask };
			}

			// Define null == null
			constexpr bool operator==(null) const noexcept {
				return true;
			}

			// Define null != null
			constexpr bool operator!=(null) const noexcept {
				return false;
			}

			// Define null == Entity
			template<typename Entity>
			constexpr bool operator==(const Entity entity) const noexcept {
				// A null is equal to an Entity if they have the same Entity ID in
				// order to coincide with conversion operator logic
				return ((to_integral(entity) & traits<Entity>::entity_mask) == traits<Entity>::entity_mask);
				//        ^-- pull the entity id out via bitmask --^
			}

			// Define null != Entity
			template<typename Entity>
			constexpr bool operator!=(const Entity entity) const noexcept {
				return !(*this == entity);
			}

		};
		// Now, we have to define the == and != operator with rhs null. 

		// Define Entity == null
		template<typename Entity>
		constexpr bool operator==(const Entity entity, const null other) noexcept {
			return other == entity;
		}

		// Define Entity != null
		template<typename Entity>
		constexpr bool operator!=(const Entity entity, const null other) noexcept {
			return other != entity;
		}

	}

	// Use null as an alias to an instantiation of the null class
	constexpr auto null = internal::null{};
}
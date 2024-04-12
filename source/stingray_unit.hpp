// Copyright 2024 Michael Fabian 'Xaymar' Dirks <info@xaymar.com>
//
// Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
// 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS” AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once
#include <cinttypes>
#include <cstddef>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <string>
#include "stingray.hpp"
#include "stingray_data.hpp"

namespace stingray {
	namespace unit {
		enum class element_type : uint32_t {
			POSITION = 0,
			_1       = 1,
			_2       = 2,
			_3       = 3,
			TEXCOORD = 4,
			// This is usually always 0xFFFFFFFF
			_5 = 5,
			// These two are only present with a .bones file.
			_BONES_6 = 6,
			_BONES_7 = 7,
		};

		enum class element_format : uint32_t {
			F32VEC2 = 1,
			F32VEC3 = 2,

			_4_4B_WIDE  = 4,
			_24_4B_WIDE = 24,
			_25_4B_WIDE = 25,
			_26_4B_WIDE = 26,

			F16VEC2 = 29,
		};

		struct datatype_t {
			uint32_t magic;
			uint32_t __unk0;

			struct element_t {
				element_type   type;
				element_format format;
				uint32_t       layer;
				uint32_t       __unk0;
				uint32_t       __unk1;
			} element[16];
			uint32_t elements;
			uint32_t __unk1;

			struct {
				uint32_t magic;
				uint32_t __unk0;
				uint32_t __unk1;
				uint32_t __unk2;
				uint32_t vertices;
				uint32_t vertex_stride;
				uint32_t __unk3;
				uint32_t __unk4;
				uint32_t __unk5;
				uint32_t __unk6;
			} __unk2;

			struct {
				uint32_t magic;
				uint32_t __unk00;
				uint32_t __unk01;
				uint32_t __unk02;
				uint32_t indices;
				uint32_t __unk03;
				uint32_t __unk04;
				uint32_t __unk05;
				uint32_t __unk06;
				uint32_t __unk07;
				uint32_t vertex_offset;
				uint32_t vertex_size;
				uint32_t index_offset;
				uint32_t index_size;
				uint32_t __unk08;
				uint32_t __unk09;
				uint32_t __unk10;
				uint32_t __unk11;
			} __unk3;
		};

		class mesh {
			public:
			struct group_t {
				uint32_t __unk0;
				uint32_t vertex_offset;
				uint32_t vertices;
				uint32_t index_offset;
				uint32_t indices;
				uint32_t __unk1;
			};

			struct data_t {
				uint32_t              __unk01;
				uint32_t              __unk02;
				uint32_t              __unk03;
				uint32_t              __unk04;
				uint32_t              __unk05;
				uint32_t              __unk06;
				uint32_t              __unk07;
				uint32_t              __unk08;
				uint32_t              __unk09;
				stingray::thin_hash_t node;
				uint32_t              __unk10;
				uint32_t              __unk11;
				uint32_t              __unk12;
				uint32_t              __unk13;
				uint32_t              datatype_index;
				uint32_t              __unk14;
				uint32_t              __unk15;
				uint32_t              __unk16;
				uint32_t              __unk17;
				uint32_t              __unk18;
				uint32_t              __unk19;
				uint32_t              __unk20;
				uint32_t              __unk21;
				uint32_t              __unk22;
				uint32_t              __unk23;
				uint32_t              materials;
				uint32_t              material_offset;
				uint32_t              __unk24;
				uint32_t              __unk25;
				uint32_t              groups;
				uint32_t              group_offset;

				// at this+material_offset
				// stingray::thin_hash_t material[materials];

				// at this+group_offset
				// mesh_group_t group[groups];
			};

			private:
			data_t const*                _ptr;
			stingray::thin_hash_t const* _material_ptr;
			group_t const*               _group_ptr;

			public:
			~mesh();
			mesh();
			mesh(uint8_t const* ptr);

			size_t datatype();

			size_t materials();

			stingray::thin_hash_t material(size_t idx);

			size_t groups();

			group_t const* group(size_t idx);
		};

		class mesh_list {
			public:
			struct data_t {
				uint32_t count;
				//uint32_t offsets[count];
				//stingray::thin_hash_t names[count];
				//mesh_t meshes[count];
			};

			private:
			data_t const*                _ptr;
			uint32_t const*              _offset_ptr;
			stingray::thin_hash_t const* _name_ptr;
			uint8_t const*               _mesh_ptr;

			std::vector<std::shared_ptr<mesh>>                     _meshes;
			std::map<stingray::thin_hash_t, std::shared_ptr<mesh>> _map;

			public:
			~mesh_list();
			mesh_list();
			mesh_list(uint8_t const* ptr);

			size_t size();

			std::shared_ptr<mesh> at(size_t idx);

			std::map<stingray::thin_hash_t, std::shared_ptr<mesh>> get();
		};

		class node_list {
			public:
			struct header_t {
				uint32_t count;
				uint32_t __unk00;
				uint32_t __unk01;
				uint32_t __unk02;

				// trss_t trss[count];
				// float precalc[4 * 4 * count];
				// link_t links[count]
				// stingray::thinhash_t node[count];
			};

			struct trss_t {
				float rotation[3 * 3];
				float translation[3];
				float scale[3];
				float skew;
			};

			struct link_t {
				uint16_t __unk;
				uint16_t parent;
			};

			struct meta_t {
				stingray::thin_hash_t name;
				trss_t const*         trss;
				link_t const*         link;
			};

			private:
			header_t const*              _ptr;
			trss_t const*                _trss_ptr;
			link_t const*                _link_ptr;
			stingray::thin_hash_t const* _name_ptr;

			std::vector<std::shared_ptr<meta_t>>                     _metas;
			std::map<stingray::thin_hash_t, std::shared_ptr<meta_t>> _map;

			public:
			~node_list();
			node_list();
			node_list(uint8_t const* ptr);

			size_t size();

			std::shared_ptr<meta_t> at(size_t idx);

			std::map<stingray::thin_hash_t, std::shared_ptr<meta_t>> const& get();
		};

		class material_list {
			public:
			struct header_t {
				uint32_t count;
				//stingray::thin_hash_t keys[count];
				//stingray::hash_t values[count];
			};

			private:
			header_t const*              _ptr;
			stingray::thin_hash_t const* _key_ptr;
			stingray::hash_t const*      _value_ptr;

			std::map<stingray::thin_hash_t, stingray::hash_t> _map;

			public:
			~material_list();
			material_list();
			material_list(uint8_t const* ptr);

			std::map<stingray::thin_hash_t, stingray::hash_t> const& get();
		};

		class unit {
			public:
			struct data_t {
				uint32_t         __unk00;
				uint32_t         __unk01;
				stingray::hash_t bones;
				uint32_t         __unk02;
				uint32_t         __unk03;
				stingray::hash_t __unkhash00;
				stingray::hash_t state_machine;
				uint32_t         __unk04;
				uint32_t         __unk05;
				uint32_t         __unkoffset0;
				uint32_t         nodes_offset;
				uint32_t         __unkoffset2;
				uint32_t         __unkoffset3;
				uint32_t         __unk06;
				uint32_t         __unk07;
				uint32_t         __unk08;
				uint32_t         __unkoffset4;
				uint32_t         __unkoffset5;
				uint32_t         __unk09;
				uint32_t         __unkoffset6;
				uint32_t         datatypes_offset;
				uint32_t         meshdata_offset;
				uint32_t         meshinfo_offset;
				uint32_t         __unk10;
				uint32_t         __unk11;
				uint32_t         materials_offset;
			};

			private:
			stingray::data_110000F0::meta_t _meta;

			data_t const* _ptr;
			material_list _material_list;

			uint8_t const* _data;
			size_t         _data_sz;

			public:
			~unit();
			unit(stingray::data_110000F0::meta_t meta);

			size_t size();

			std::string extension();

			std::list<std::pair<void const*, size_t>> sections();
		};
	} // namespace unit
} // namespace stingray

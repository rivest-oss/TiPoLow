/*******************************************************************************
 *
 *	Project name:
 *		TiPoLow
 *	
 *	File name:
 *		codec-qoi.h
 *	
 *	Description:
 *		[TODO]
 *	
 *	Exposed:
 *		* [TODO]
 *
 ******************************************************************************/

#ifndef __tipolow__qoi__h__
#define __tipolow__qoi__h__

#include "image.h"

namespace TiPoLow {
	namespace CodecQOI {
		class Decoder : private Image {
			public:
				typedef struct {
					u32 width = -1;
					u32 height = -1;
					
					Image *img_ptr = nullptr;
					
					// 3 = RGB, 4 = RGBA.
					u8 channels = -1;
					
					// 0 = sRGB with linear alpha.
					// 1 = All channels are linear.
					u8 colorspace = -1;
				} image_t;
			
			private:
				BaseIOStream *rs = nullptr;
				image_t *img = nullptr;
				bool is_header_ready = false;
				
				u8 pixels_index[256];
				u32 pixel_i = 0;
				u32 pixels = 0;
				
				ErrorOr<void> read_header(void) {
					ErrorOr<u32> e_sign = rs->readU32BE();
					if(e_sign.error) return Error { e_sign.error };
					
					if(e_sign.value() != 0x716f6966)
						return Error { "Invalid signature" };
					
					ErrorOr<u32> e_width = rs->readU32BE();
					if(e_width.error) return Error { e_width.error };
					
					ErrorOr<u32> e_height = rs->readU32BE();
					if(e_height.error) return Error { e_height.error };
					
					ErrorOr<u8> e_channels = rs->readU8();
					if(e_channels.error) return Error { e_channels.error };
					
					if((e_channels.value() < 3) | (e_channels.value() > 4))
						return Error { "Invalid channels count" };
					
					ErrorOr<u8> e_colorspace = rs->readU8();
					if(e_colorspace.error)
						return Error { e_colorspace.error };
					
					if(e_colorspace.value() > 2)
						return Error { "Invalid colorspace" };
					
					img->width = e_width.value();
					img->height = e_height.value();
					img->channels = e_channels.value();
					img->colorspace = e_colorspace.value();
					
					img->img_ptr->width(img->width);
					img->img_ptr->height(img->height);
					
					ErrorOr<void> e_img = img->img_ptr->init();
					if(e_img.error) return Error { e_img.error };
					
					is_header_ready = true;
					pixels = (img->width * img->height);
					// [FIXME] ^^^^ We should really check this for overflow...
					
					return Error { nullptr };
				};
				
				inline u16 get_index_pos(u8 r, u8 g, u8 b, u8 a) {
					return (((r * 3 + g * 5 + b * 7 + a * 11) & 0x3f) << 2);
				};
				
				ErrorOr<bool> read_packet(void) {
					// [TODO]
					return Error { "[TODO]" };
				};
			
			public:
				ErrorOr<void> init(BaseIOStream* rstream) {
					ErrorOr<void> e_de = deinit();
					if(e_de.error) return Error { e_de.error };
					
					rs = rstream;
					
					ErrorOr<image_t*> e_img = alloc_type<image_t>(1);
					if(e_img.error) return Error { e_img.error };
					
					img = e_img.value();
					
					for(u16 i = 0; i < 256; i++)
						pixels_index[i] = 0x00;
					
					pixel_i = 0;
					
					return Error { nullptr };
				};
				
				ErrorOr<void> free(void) {
					if(img != nullptr) {
						if(img->img_ptr != nullptr) {
							ErrorOr<void> e = free_type(img->img_ptr);
							if(e.error) return Error { e.error };
						}
						
						ErrorOr<void> e = free_type(img);
						if(e.error) return Error { e.error };
					}
					
					return Error { nullptr };
				};
				
				// This does not frees the allocated image.
				ErrorOr<void> deinit(void) {
					rs = nullptr;
					is_header_ready = false;
					
					return Error { nullptr };
				};
				
				// Returns (false) if there is no more data to process,
				// returns (true) otherwise.
				ErrorOr<bool> decode(void) {
					// Read header.
					if(is_header_ready == false) {
						ErrorOr<void> e = read_header();
						return Error { e.error };
					}
					
					return read_packet();
				};
				
				image_t *image(void) { return img; };
		};
	};
};

#endif


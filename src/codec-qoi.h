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
				bool is_header_ready = false, stop_already = false;
				
				u8 pixels_index[256];
				u32 pixel_i = 0;
				u32 pixels = 0;
				
				u8 r, g, b, a;
				
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
				
				u16 get_index_pos(void) {
					return ((r * 3 + g * 5 + b * 7 + a * 11) & 0x3f);
				};
				
				ErrorOr<bool> read_packet(void) {
					if(stop_already) return false;
					
					ErrorOr<u8> e_byte = rs->readU8();
					if(e_byte.error) return Error { e_byte.error };
					
					// If detects that all pixels have been written, it should
					// check if the following eight bytes are:
					// * seven (0x00) bytes.
					// * one (0x01) byte.
					// 
					// If they are not those bytes, return error.
					// If it's not EOF, don't care, only care if the last bytes
					// are the right ones.
					
					if((e_byte.value() & 0xfe) == 0xfe) {
						// QOI_OP_RGB or QOI_OP_RGBA.
						bool has_alpha_component = (e_byte.value() & 0x01);
						
						// QOI_OP_RGB
						e_byte = rs->readU8();
						if(e_byte.error) return Error { e_byte.error };
						r = e_byte.value();
						
						e_byte = rs->readU8();
						if(e_byte.error) return Error { e_byte.error };
						g = e_byte.value();
						
						e_byte = rs->readU8();
						if(e_byte.error) return Error { e_byte.error };
						b = e_byte.value();
						
						if(has_alpha_component) {
							e_byte = rs->readU8();
							if(e_byte.error) return Error { e_byte.error };
							a = e_byte.value();
						}
					} else {
						switch(e_byte.value() >> 6) {
							case 0b00: {
								// QOI_OP_INDEX
								u16 i = ((e_byte.value() & 0x3f) << 2);
								
								r = pixels_index[i | 0x0];
								g = pixels_index[i | 0x1];
								b = pixels_index[i | 0x2];
								a = pixels_index[i | 0x3];
								
								break;
							};
							
							case 0b01: {
								// QOI_OP_DIFF
								i16 dr = (-2), dg = (-2), db = (-2);
								
								dr += (i16)((e_byte.value() & 0x30) >> 4);
								dg += (i16)((e_byte.value() & 0x0c) >> 2);
								db += (i16)(e_byte.value() & 0x03);
								
								r += dr;
								g += dg;
								b += db;
								
								break;
							};
							
							case 0b10: {
								// QOI_OP_LUMA
								
								i16 dr = (-8), dg = (-32), db = (-8);
								dg += (i16)(e_byte.value() & 0x3f);
								
								e_byte = rs->readU8();
								if(e_byte.error) return Error { e_byte.error };
								
								dr += (i16)(e_byte.value() >> 4);
								db += (i16)(e_byte.value() & 0x0f);
								
								dr += dg;
								db += db;
								
								r += dr;
								g += dg;
								b += db;
								
								break;
							};
							
							case 0b11: {
								// QOI_OP_RUN
								
								u16 rep_n = (u16)(e_byte.value() & 0x3f);
								
								for(u16 i = 0; i < rep_n; i++) {
									u8 *p = img->img_ptr->data_pointer();
									
									p[(pixel_i << 2) | 0x0] = r;
									p[(pixel_i << 2) | 0x1] = g;
									p[(pixel_i << 2) | 0x2] = b;
									p[(pixel_i << 2) | 0x3] = a;
									
									pixel_i++;
									
									if(pixel_i >= pixels) {
										stop_already = true;
										return false;
									}
								};
								
								break;
							};
						};
					}
					
					u16 i = (get_index_pos() << 2);
					pixels_index[i | 0x0] = r;
					pixels_index[i | 0x1] = g;
					pixels_index[i | 0x2] = b;
					pixels_index[i | 0x3] = a;
					
					img->img_ptr->data_pointer()[(pixel_i << 2) | 0x0] = r;
					img->img_ptr->data_pointer()[(pixel_i << 2) | 0x1] = g;
					img->img_ptr->data_pointer()[(pixel_i << 2) | 0x2] = b;
					img->img_ptr->data_pointer()[(pixel_i << 2) | 0x3] = a;
					
					pixel_i++;
					
					if(pixel_i >= pixels) {
						stop_already = true;
						return false;
					}
					
					return true;
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
					r = 0x00, g = 0x00, b = 0x00, a = 0xff;
					
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
					stop_already = false;
					
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


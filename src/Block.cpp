#include "Block.h"

Block::Block(SDL_Rect* b, bool ib, bool f, bool iw, short cr, short cg, short cb) : 
block_rect{b}, is_bouncy{ib}, is_frictionless{f}, is_winning{iw}, color_r{cr}, color_g{cg}, color_b{cb}
{

}
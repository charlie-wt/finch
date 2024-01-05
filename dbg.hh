#pragma once

#include "geom.hh"
#include "text.hh"


void init_dbg ();
void dbg (std::wstring const &text);
void draw_dbg ();
void dbg_pos (pixel p);
pixel dbg_pos ();
TextCanvas &dbg_canvas ();

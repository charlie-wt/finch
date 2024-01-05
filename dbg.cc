#include "dbg.hh"

#include "term.hh"

#include <vector>


namespace {

TextCanvas _canv({ 0, 0 });
pixel _pos { 1, 1 };
std::vector<std::wstring> _lines;

}

void init_dbg () {
    _canv = TextCanvas(TermInfo::detect());
}

void dbg (std::wstring const &text) {
    _lines.push_back(text);
}

void draw_dbg () {
    for (size_t i = 0; i < _lines.size(); i++) {
        _canv.write(_pos.x(), _pos.y() + i,
                    _lines[i]);
    }
    _canv.draw();

    _canv.clear();
    _lines.clear();
}

void dbg_pos (pixel p) {
    _pos = p;
}

pixel dbg_pos () {
    return _pos;
}

TextCanvas &dbg_canvas () {
    return _canv;
}

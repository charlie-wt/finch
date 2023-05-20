#include "termvis.hh"
#include <iostream>

using namespace std;


int main () {
    string const c = "\u2801 \u2802 \u2803 \u28ff";
    cout << "hello" << c << "world.\n";

    Block b { 0b11011011 };
    wcout << "this is a test: " << b.to_char() << "\n";

    CharGrid cg(TermInfo::detect());

    vector<Line> lines;
    lines.push_back({ { 0., 0. }, { 50., 5. } });
    lines.push_back({ { 0., 0. }, { 5., 20. } });
    lines.push_back({ { 0., 0. }, { 20., 20. } });
    lines.push_back({ { 40., 20. }, { 10., 13. } });

    /* int count = 0; */
    /* while (true) { */
    /*     cg.clear(); */

    /*     for (auto const &l : lines) { */
    /*         l.draw(cg); */
    /*     } */

    /*     cg.draw(); */

    /*     sleep(100); */
    /*     count++; */
    /* } */
}
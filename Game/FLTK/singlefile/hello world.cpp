#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>

int main(int argc, char **argv) {
    Fl_Window *window = new Fl_Window(600,800);
    Fl_Box *box = new Fl_Box(300-260/2,400-100/2,260,100,"Hello, world!");
    box->box(FL_UP_BOX);
    box->labelsize(36);
    box->labelfont(FL_BOLD+FL_ITALIC);
    box->labeltype(FL_SHADOW_LABEL);		
    window->end();
    window->show(argc, argv);    
    return Fl::run();
}
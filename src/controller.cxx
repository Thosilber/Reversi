#include "controller.hxx"

Controller::Controller(int size)
        : Controller(size, size)
{ }

Controller::Controller(int width, int height)
        : model_(width, height),
          view_(model_),
          mouse_{0,0}
{ }


void
Controller::on_mouse_move(ge211::Posn<int> pos)
{
    mouse_ = pos;
}

void
Controller::on_mouse_up(ge211::Mouse_button click, ge211::Posn<int> pos)
{
    // clicking the button
    if(click == ge211::Mouse_button::left) {
        // checking if its a valid move
        if(model_.find_move(view_.screen_to_board(pos))) {
            // if a valid move, then play it (this avoids game from crashing)
            model_.play_move(view_.screen_to_board(pos));
        }
    }
}


void
Controller::draw(ge211::Sprite_set& sprites)
{
    view_.draw(sprites, mouse_);
}

View::Dimensions
Controller::initial_window_dimensions() const
{
    return view_.initial_window_dimensions();
}

std::string
Controller::initial_window_title() const
{
    return view_.initial_window_title();
}


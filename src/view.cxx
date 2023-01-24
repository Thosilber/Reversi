#include "view.hxx"

// Convenient type aliases:
using Color = ge211::Color;
using Sprite_set = ge211::Sprite_set;

// You can change this or even determine it some other way:
static int const grid_size = 36;

View::View(Model const& model)
        : model_(model),
          green_box_sprite_({(int)(grid_size * .90), (int)(grid_size * .90)},
                           Color::from_rgba(0,150,0)),
          red_box_sprite_({(int)(grid_size * .90), (int)(grid_size * .90)},
                         Color::from_rgba(150,0,0)),
          grey_box_sprite_({(int)(grid_size * .90), (int)(grid_size * .90)},
                          Color::from_rgba(75,75,75)),
          dark_ball_sprite_((grid_size * .45), Color::black()),
          light_ball_sprite_((grid_size * .45), Color::white()),
          small_dark_ball_sprite_((grid_size * .15), Color::black()),
          small_light_ball_sprite_((grid_size * .15), Color::white())


        // You may want to add sprite initialization here

{ }

void View::draw(Sprite_set& set, Position user)
{

    // using mouse to determine position

    // call find_move at a specific mouse position to find all moves that are
    // possible at that position
    Move const *moves_available = model_.find_move(screen_to_board(user));


    // if there are moves available, put those on a Position_set
    Position_set p;
    if(moves_available != nullptr) {
        p = moves_available->second; //->second puts it a position set form
    }


    // TODO, PROBABLY

    // loop over board
    for(Position pos : model_.board()) {

        // add all green tiles
        set.add_sprite(green_box_sprite_, board_to_screen(pos), 1);

        // add player tiles
        add_player_sprite_(set,model_[pos], pos, 5);

        // add the red tiles around the tiles that would be "acquired" by player
        // MY CODE IS CRASHING HERE (not anymore)
        if(p[pos]) {  // check if this position is one of
                                            // the viable moves gotten from find_move
            set.add_sprite(red_box_sprite_, board_to_screen(pos), 3);
        }

        // add the grey tiles on the winner when the game is over
        /*
        if (model_[pos] != model_.winner() && model_.is_game_over()){
            set.add_sprite(grey_box_sprite_, board_to_screen(pos),4);
        }
        */
        if(model_.is_game_over()) {
            add_player_sprite_(set,model_[pos], pos, 5);
        }

        /*
        if(user.x == board_to_screen(pos).x && user.y == board_to_screen(pos)
        .y){ // trying to put the small ball in the center of each tile
            if (model_.turn() == Player::dark) {
                set.add_sprite(small_dark_ball_sprite_, pos, 2);
            }
            if (model_.turn() == Player::light){
                set.add_sprite(small_light_ball_sprite_, pos, 2);
            }
        }
        */
    }


    // make user movements and view interact with one another
    // add a little ball on the tiles being selected by user
    // BALL IS FOLLOWING THE MOUSE MOVEMENT AND NOT BEING PLACE INSIDE BOXES...
    if (model_.turn() == Player::dark) {
        set.add_sprite(small_dark_ball_sprite_, user, 2);
    }
    if (model_.turn() == Player::light){
        set.add_sprite(small_light_ball_sprite_, user, 2);
    }
}


View::Dimensions
View::initial_window_dimensions() const
{
    // You can change this if you want:
    return grid_size * model_.board().dimensions();
}

std::string
View::initial_window_title() const
{
    // You can change this if you want:
    return "Reversi";
}

View::Position
View::board_to_screen(Model::Position pos) const
{
    return {grid_size * pos.x, grid_size * pos.y};
}

Model::Position
View::screen_to_board(View::Position pos) const
{
    return {pos.x / grid_size, pos.y / grid_size};
}

void
View::add_player_sprite_(
        Sprite_set& set,
        Player player,
        Position pos,
        int z) const
{
    // TODO

    if(!model_.is_game_over()){
        if(model_[pos] == Player::dark) {
            set.add_sprite(dark_ball_sprite_, board_to_screen(pos), z);
        }

        if(model_[pos] == Player::light) {
            set.add_sprite(light_ball_sprite_, board_to_screen(pos), z);
        }
    }

    else{
        if(model_[pos] != model_.winner()) {
            set.add_sprite(grey_box_sprite_, board_to_screen(pos), z-1);
        }
        if(model_[pos] == Player::dark) {
            set.add_sprite(dark_ball_sprite_, board_to_screen(pos), z);
        }

        if(model_[pos] == Player::light) {
            set.add_sprite(light_ball_sprite_, board_to_screen(pos), z);
        }
    }
    // ^^ adding this here -> if causes problems, just remove

}
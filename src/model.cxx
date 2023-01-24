#include "model.hxx"

using namespace ge211;

Model::Model(int size)
        : Model(size, size)
{ }

Model::Model(int width, int height)
        : board_({width, height})
{
    if(width >= 2 && width <= 8 && height >= 2 && height <= 8){
        compute_next_moves_();
    }
    else{
        throw ge211::Client_logic_error("ERROR");
    }
    // TODO: initialize `next_moves_` to `turn_`'s available moves
    //compute_next_moves_();
    // do I need more here?
}

Model::Rectangle
Model::board() const
{
    return board_.all_positions();
}

Player
Model::operator[](Position pos) const
{
    return board_[pos];
}

const Move*
Model::find_move(Position pos) const
{
    auto i = next_moves_.find(pos);

    if (i == next_moves_.end()) {
        // Nothing was found, so return NULL (nullptr in C++)
        return nullptr;
    } else {
        // Dereferences the iterator to get the value then returns a pointer
        // to that value. This is safe as the pointer is to a value living
        // within the `next_moves_` structure.
        return &(*i);
    }
}

void
Model::play_move(Position pos)
{
    if (is_game_over()) {
        throw Client_logic_error("Model::play_move: game over");
    }

    const Move* movep = find_move(pos);
    if (movep == nullptr) { // check if there was no such move
        throw Client_logic_error("Model::play_move: no such move");
    }

    // TODO: actually execute the move, advance the turn, refill
    // `next_moves_`, etc.

    really_play_move_(*movep);
}

//
// BELOW ARE HELPER FUNCTIONS
// Our tests will run on the public functions, not the helper functions
// However, you are required to implement the following functions to help the
// public functions
//

Position_set
Model::find_flips_(Position current, Dimensions dir) const
{
    // TODO: HELPER FUNCTION

    // create an empty position set that will be filled
    Position_set p;
    // create an empty position set that will remain empty
    Position_set r;

    // start adding positions to it one-by-one as long as the position is in
    // bounds and contains the opposite player


    if(board_[current] == Player::neither) {
        current = current + dir; // move to the next


        // iterate move by move in the direction until:
        // go out of bounds
        // reach a position that does not contain the opposite player

        /*
        while (board_.good_position(current) && board_[current] !=
                                                Player::neither) {
            if (board_[current] == other_player(turn_)) {
                p[current] = true;
                current = current + dir;
            } else {
                return p;
            }
        }
        */
        while(true) {
            if (board_.good_position(current) && board_[current] !=
                                                 Player::neither) {
                if (board_[current] == other_player(turn_)) {
                    p[current] = true;
                    current = current + dir;
                } else {
                    return p;
                }
            }
            else{
                return r;
            }
        }
    }
    return r;
}

Position_set
Model::evaluate_position_(Position pos) const
{
    // TODO: HELPER FUNCTION

    // loop over each board direction
    // for each direction call fin_flips_() to fid the positions that would flip
    // keep track of these positions in a position_set object

    Position_set p;

    for (Dimensions dir : Board::all_directions()){
        p |= find_flips_(pos,dir);
    }

    // return all viable moves
    return p;

}

void
Model::compute_next_moves_()
{
    // TODO: HELPER FUNCTION

    // clear out next_moves_
    next_moves_.clear();

    // two cases:
    // opening phase or main phase
    // check that by seeing if there are more than 4 "plays"

    if (board_.count_player(Player::light) + board_.count_player
    (Player::dark) >= 4) {
        for (Position pos: board_.all_positions()) {
            // call evaluate positions
            if (!evaluate_position_(pos).empty()) {
                // combine all positions yielded by evaluate position and the
                // current position
                next_moves_[pos] = (evaluate_position_(pos) |=
                                            Position_set{pos});
            }
        }
    }
    else { // when there are less than 4 plays
        for(Position pos : board_.center_positions()) {
            if (board_[pos] == Player::neither) {
                // set the move to be the tile being selected
                next_moves_[pos] = Position_set{pos};
            }
        }
    }
}

bool
Model::advance_turn_()
{
    // TODO: HELPER FUNCTION

    // switch to the other player
    turn_ = other_player(turn_);

    // Compute the set of viable moves for the next player
    compute_next_moves_();

    // Return if any moves are available (if next_moves is empty - return false)
    if(next_moves_.empty()){
        return false;
    }
    else {
        return true;
    }
}

void
Model::set_game_over_()
{
    // TODO: HELPER FUNCTION
    // set player (turn_) equal to neither
    turn_ = Player::neither;

    // count the number of tiles occupied by each player
    int dark_tiles = board_.count_player(Player::dark);
    int light_tiles = board_.count_player(Player::light);

    // set the winner accordingly (based on who has more tiles)
    if(dark_tiles > light_tiles) {
        winner_ = Player::dark;
    }
    else{
        if(dark_tiles < light_tiles){
            winner_ = Player::light;
        }
    }
}

void
Model::really_play_move_(Move move)
{
    // TODO: HELPER FUNCTION
    // set each position in move.second to the current player
    board_.set_all(move.second, turn_);

    // check if game is over:
    // call advance_turn_() twice and both times return false -> game is over
    if(!advance_turn_()){
        if(!advance_turn_()){
            set_game_over_();
        }
    }
}

#include "../includes/cub.h"

int main(int argc, char **argv)
{
    t_game *data;

    if (argc != 2)
        return (1);
    data = init_game();
    if (!data)
        return (1);
    
    if (process_map(data, argv[1]) == false)
        return (1);

	// Interface functions to raycast section
	/* Somewhere here, after collection the info from the map:
	 * (maybe in a subfunction of course)
	 * init_player_field and cdir could but don't have to be stored somewhere in data,
	 */
	// set_initial_player_pos(&data->player, t_fidx init_player_field, t_cdir cdir);

	/* ... and then later in each update loop after evaluating the user input
	*/
	// update_player_pos(
	// 	&data->player, t_coord const deltapos, double const deltadov);
return (0);
}
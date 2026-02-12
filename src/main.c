/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 09:15:11 by rhaas             #+#    #+#             */
/*   Updated: 2026/02/11 16:46:44 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub.h"

int	main(int argc, char **argv)
{
	t_parser	parser;
	t_game		data;
	int			filenamestrlen;

	if (argc != 2)
		clean_exit(NULL, 1,
			"Exactly one argument (filename of scene/map file) must be given.");
	filenamestrlen = ft_strlen(argv[1]);
	if (filenamestrlen < 4 || ft_strncmp(argv[1] + (filenamestrlen - 4),
			".cub", filenamestrlen) != 0)
		clean_exit(NULL, 1,
			"The filename of scene/map file must have the extension '.cub'.");
	init_parser(&parser);
	if (process_map(&parser, argv[1]) == false)
		clean_exit(parser.parse_memory, 1, "file does not exist");
	if (verify_map(&parser) == false)
		clean_exit(parser.parse_memory, 1, "invalid map");
	init_data(&data, &parser);
	start_game(&data);
	clean_memory_list(&parser.parse_memory);
	return (0);
}

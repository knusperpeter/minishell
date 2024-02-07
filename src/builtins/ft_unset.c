/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 23:49:45 by caigner           #+#    #+#             */
/*   Updated: 2024/02/07 16:58:58 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
/* The unset  builtin is used to destroy arrays.  unset name[subscript] 
destroys the array element at index subscript, for both indexed and 
associative arrays.  Negative subscripts to indexed arrays are interpreted 
as described above.  Unsetting the last element of an array variable does 
not unset the variable.  unset name, where name is an array, or unset 
name[subscript], where  subscript is * or @, removes the entire array.
When  using a variable name with a subscript as an argument to a command, 
such as with unset, without using the word expansion syntax described above, 
the argument is subject to pathname expansion.  If pathname expansion is not 
desired, the argument should be quoted. */

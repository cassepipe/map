/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enable_if.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cassepipe <norminet@42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 11:42:15 by cassepipe         #+#    #+#             */
/*   Updated: 2022/07/06 11:42:15 by cassepipe        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENABLE_IF_HPP
#define ENABLE_IF_HPP

//Oh by the way, in c++98, only class/struct templates may have default parameters
template < bool Cond, typename T = void >
struct enable_if
{
};

template <typename T>
struct enable_if<true, T>
{
	T value;
};

#endif /* ENABLE_IF_HPP */

/*
 * Hdf5Observer.cpp
 *
 *  Created on: Jan 12, 2015
 *      Author: cady
 */

#include "h5_version.hpp"
#include "Hdf5Observer.hpp"

Hdf5Observer::Hdf5Observer(const std::string& filename, const std::vector<std::string>& d) : Observer(d), h5File(H5::H5File(filename,H5F_ACC_TRUNC))
{
    h5_writeFileDescription(h5File);
}

std::function<void()> Hdf5Observer::get_serializer(const double, const std::vector<std::string>& , const std::string& )
{
    return [](){};
}

std::function<void()> Hdf5Observer::get_serializer(const std::string&, const std::vector<std::string>& , const std::string& )
{
    return [](){};
}

std::function<void()> Hdf5Observer::get_serializer(const std::vector<double>& , const std::vector<std::string>& , const std::string& )
{
    return [](){};
}

std::function<void()> Hdf5Observer::get_serializer(const std::vector<std::vector<double> >& , const std::vector<std::string>& , const std::string& )
{
    return [](){};
}

std::function<void()> Hdf5Observer::get_initializer(const double , const std::vector<std::string>& , const std::string&)
{
    return [](){};
}

std::function<void()> Hdf5Observer::get_initializer(const std::string& , const std::vector<std::string>& , const std::string&)
{
    return [](){};
}

std::function<void()> Hdf5Observer::get_initializer(const std::vector<double>& , const std::vector<std::string>& , const std::string& )
{
    return [](){};
}

std::function<void()> Hdf5Observer::get_initializer(const std::vector<std::vector<double> >& , const std::vector<std::string>& , const std::string& )
{
    return [](){};
}

void Hdf5Observer::flush_after_initialization()
{
}

void Hdf5Observer::flush_after_write()
{
}

void Hdf5Observer::flush_value()
{
}

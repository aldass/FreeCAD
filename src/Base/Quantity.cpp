/***************************************************************************
 *   Copyright (c) 2013 Juergen Riegel                                     *
 *                                                                         *
 *   This file is part of the FreeCAD CAx development system.              *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License as published by the Free Software Foundation; either          *
 *   version 2 of the License, or (at your option) any later version.      *
 *                                                                         *
 *   This library  is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this library; see the file COPYING.LIB. If not,    *
 *   write to the Free Software Foundation, Inc., 59 Temple Place,         *
 *   Suite 330, Boston, MA  02111-1307, USA                                *
 *                                                                         *
 ***************************************************************************/

#include "PreCompiled.h"
#ifndef _PreComp_
# include <sstream>
#endif

#include <cmath>
#include "Quantity.h"
#include "Exception.h"
#include "UnitsApi.h"

// suppress annoying warnings from generated source files
#ifdef _MSC_VER
# pragma warning(disable : 4003)
# pragma warning(disable : 4018)
# pragma warning(disable : 4065)
# pragma warning( disable : 4273 )
# pragma warning(disable : 4335) // disable MAC file format warning on VC
#endif


using namespace Base;

Quantity::Quantity()
{
    this->_Value = 0.0;
}

Quantity::Quantity(const Quantity& that)
{
    *this = that ;
}

Quantity::Quantity(double Value, const Unit& unit)
{
    this->_Unit = unit;
    this->_Value = Value;
}


bool Quantity::operator ==(const Quantity& that) const
{
    return (this->_Value == that._Value) && (this->_Unit == that._Unit) ;
}


Quantity Quantity::operator *(const Quantity &p) const
{
    return Quantity(this->_Value * p._Value,this->_Unit * p._Unit);
}
Quantity Quantity::operator /(const Quantity &p) const
{
    return Quantity(this->_Value / p._Value,this->_Unit / p._Unit);
}

Quantity Quantity::pow(const Quantity &p) const
{
    if(!p._Unit.isEmpty()) 
        throw Base::Exception("Quantity::pow(): exponent must not have a unit");
    return Quantity(
        std::pow(this->_Value, p._Value),
        this->_Unit.pow((short)p._Value)
        );
}


Quantity Quantity::operator +(const Quantity &p) const
{
    if(this->_Unit != p._Unit) 
        throw Base::Exception("Quantity::operator +(): Unit missmatch in plus operation");
    return Quantity(this->_Value + p._Value,this->_Unit);
}
Quantity Quantity::operator -(const Quantity &p) const
{
    if(this->_Unit != p._Unit) 
        throw Base::Exception("Quantity::operator +(): Unit missmatch in plus operation");
    return Quantity(this->_Value - p._Value,this->_Unit);
}

Quantity Quantity::operator -(void) const
{
    return Quantity(-(this->_Value),this->_Unit);
}

Quantity& Quantity::operator = (const Quantity &New)
{
    this->_Value = New._Value;
    this->_Unit = New._Unit;
    return *this;
}

double Quantity::getUserPrefered(QString &unitString)const
{
	return Base::UnitsApi::schemaPrefUnit(_Unit,unitString).getValue() * _Value;
}

std::string Quantity::getUserString(void)const
{
    std::stringstream sstream;
    sstream << _Value << _Unit.getString();
    //TODO: implementing 
    return sstream.str();
}

/// true if it has a number without a unit
bool Quantity::isDimensionless(void)const
{
    return _Value != DOUBLE_MIN && _Unit.isEmpty();
}
// true if it has a number and a valid unit
bool Quantity::isQuantity(void)const
{
    return _Value != DOUBLE_MIN && !_Unit.isEmpty();
}
// true if it has a number with or without a unit
bool Quantity::isValid(void)const
{
    return _Value != DOUBLE_MIN ;
}

void Quantity::setInvalid(void)
{
    _Value = DOUBLE_MIN ;
}

// === Predefined types =====================================================

Quantity Quantity::NanoMeter        (1.0e-6         ,Unit(1));
Quantity Quantity::MicroMeter       (1.0e-3         ,Unit(1));
Quantity Quantity::MilliMeter       (1.0            ,Unit(1));
Quantity Quantity::CentiMeter       (10.0           ,Unit(1));
Quantity Quantity::DeciMeter        (100.0          ,Unit(1));
Quantity Quantity::Meter            (1.0e3          ,Unit(1));
Quantity Quantity::KiloMeter        (1.0e6          ,Unit(1));

Quantity Quantity::Liter            (1000000.0      ,Unit(3));

Quantity Quantity::MicroGram        (1.0e-9         ,Unit(0,1));
Quantity Quantity::MilliGram        (1.0e-6         ,Unit(0,1));
Quantity Quantity::Gram             (1.0e-3         ,Unit(0,1));
Quantity Quantity::KiloGram         (1.0            ,Unit(0,1));
Quantity Quantity::Ton              (1.0e3          ,Unit(0,1));

Quantity Quantity::Second           (1.0            ,Unit(0,0,1));
Quantity Quantity::Minut            (60.0           ,Unit(0,0,1));
Quantity Quantity::Hour             (3600.0         ,Unit(0,0,1));

Quantity Quantity::Ampere           (1.0           ,Unit(0,0,0,1));   
Quantity Quantity::MilliAmpere      (0.001         ,Unit(0,0,0,1));   
Quantity Quantity::KiloAmpere       (1000.0        ,Unit(0,0,0,1));   
Quantity Quantity::MegaAmpere       (1.0e6         ,Unit(0,0,0,1));   

Quantity Quantity::Kelvin           (1.0           ,Unit(0,0,0,0,1));     
Quantity Quantity::MilliKelvin      (0.001         ,Unit(0,0,0,0,1));      
Quantity Quantity::MicroKelvin      (0.000001      ,Unit(0,0,0,0,1));      

Quantity Quantity::Mole             (1.0           ,Unit(0,0,0,0,0,1));    

Quantity Quantity::Candela          (1.0           ,Unit(0,0,0,0,0,0,1)); 

Quantity Quantity::Inch             (25.4          ,Unit(1));             
Quantity Quantity::Foot             (304.8         ,Unit(1));             
Quantity Quantity::Thou             (0.0254        ,Unit(1));             
Quantity Quantity::Yard             (914.4         ,Unit(1)); 

Quantity Quantity::Pound            (0.45359237    ,Unit(0,1)); 
Quantity Quantity::Ounce            (0.45359237    ,Unit(0,1)); 
Quantity Quantity::Stone            (6.35029318    ,Unit(0,1)); 
Quantity Quantity::Hundredweights   (50.80234544   ,Unit(0,1)); 

Quantity Quantity::Newton           (1000.0        ,Unit(1,1,-2));  // Newton (kg*m/s^2)
Quantity Quantity::KiloNewton       (1e+6          ,Unit(1,1,-2));  
Quantity Quantity::MegaNewton       (1e+9          ,Unit(1,1,-2));  
Quantity Quantity::MilliNewton      (1.0           ,Unit(1,1,-2));  

Quantity Quantity::Pascal           (0.001         ,Unit(-1,1,-2)); // Pascal (kg/m*s^2 or N/m^2) 
Quantity Quantity::KiloPascal       (1.00          ,Unit(-1,1,-2)); 
Quantity Quantity::MegaPascal       (1000.0        ,Unit(-1,1,-2)); 
Quantity Quantity::GigaPascal       (1e+6          ,Unit(-1,1,-2)); 

Quantity Quantity::PSI              (0.145038      ,Unit(-1,1,-2)); // pounds/in^2

Quantity Quantity::Watt             (1e+6          ,Unit(2,1,-3));  // Watt (kg*m^2/s^3) 
Quantity Quantity::VoltAmpere       (1e+6          ,Unit(2,1,-3));  // VoltAmpere (kg*m^2/s^3) 

Quantity Quantity::Joul             (1e+6          ,Unit(2,1,-2));  // Joule (kg*m^2/s^2) 
Quantity Quantity::NewtonMeter      (1e+6          ,Unit(2,1,-2));  // Joule (kg*m^2/s^2) 
Quantity Quantity::VoltAmpereSecond (1e+6          ,Unit(2,1,-2));  // Joule (kg*m^2/s^2) 
Quantity Quantity::WattSecond       (1e+6          ,Unit(2,1,-2));  // Joule (kg*m^2/s^2) 

Quantity Quantity::Degree           (1.0           ,Unit(0,0,0,0,0,0,0,1)); // degree         (internal standard angle)
Quantity Quantity::Radian           (180/M_PI      ,Unit(0,0,0,0,0,0,0,1)); // radian         
Quantity Quantity::Gon              (360.0/400.0   ,Unit(0,0,0,0,0,0,0,1)); // gon         




// === Parser & Scanner stuff ===============================================

// include the Scanner and the Parser for the Quantitys

Quantity QuantResult;



// error func
void Quantity_yyerror(char *errorinfo)
{  
    throw Base::Exception(errorinfo);  
}


// for VC9 (isatty and fileno not supported anymore)
//#ifdef _MSC_VER
//int isatty (int i) {return _isatty(i);}
//int fileno(FILE *stream) {return _fileno(stream);}
//#endif

namespace QuantityParser {

#define YYINITDEPTH 20
// show the parser the lexer method
#define yylex QuantityLexer
int QuantityLexer(void);

// Parser, defined in QuantityParser.y
#include "QuantityParser.c"

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// Scanner, defined in QuantityParser.l
#include "QuantityLexer.c"
#endif // DOXYGEN_SHOULD_SKIP_THIS
}

Quantity Quantity::parse(const char* buffer)
{
    // parse from buffer
    QuantityParser::YY_BUFFER_STATE my_string_buffer = QuantityParser::yy_scan_string (buffer);
    // set the global return variables
    QuantResult = Quantity(DOUBLE_MIN);
    // run the parser
    QuantityParser::yyparse ();
    // free the scan buffer
    QuantityParser::yy_delete_buffer (my_string_buffer);

    //if (QuantResult == Quantity(DOUBLE_MIN))
    //    throw Base::Exception("Unknown error in Quantity expression");
    return QuantResult;
}

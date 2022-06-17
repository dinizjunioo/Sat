#ifndef BARCHART_H
#define BARCHART_H

/*!
 * Bar Char Race project.
 *
 * @author Selan
 * @data October 5th, 2020.
 */

#include <string>
using std::string;

#include <iterator>

#include <algorithm>
using std::sort;

#include <chrono>
#include <thread>

#include <vector>
using std::vector;

#include "../libs/text_color.h"

namespace bcra {
    // The value type of a data item.
    using value_t = long;

    /// This class represents a single Bar Chart.
    class BarChart 
    {
        //=== Definition
        public:
            /// Represents a single bar information.
            struct BarItem 
            {
            //public:
                string label;    //!< Bar label.    // nome da barra (Brasil, Eua, ...)
                value_t value;   //!< Bar value.    // valor
                string category; //!< Bar category. // categoria
                BarItem( string l, value_t v, string c ) : label{l}, value{v}, category{c}
                {/*empty*/}
            };
            
            //== Data members
        public:
            /// The collection of bars items
            std::vector< BarItem > bars;
            
            std::string main_title;
            std::string info_date;
            std::string fonte_date;
            std::string time_stamp;
            value_t t = 5;

            //bars.BarItem(time_stamp, t, main_title);
           

            // vetor que guarda o nome, valor e categoria
            //  vetor guarda     nome = cidade, valor = populacao, categoria = continente = color
            //    ano,  cidade,  pais,  populacao,  continente
            //    1500,  Cairo, Egypt,        400, Middle East
            // vou usar mapa para mapear cidade e cor, aleatoriamente ou nao
        private:

            /// The date (timestamp) of the bar chart.
            std::string m_date;

            //== Public interface
        public:
            /// Default ctro.
            //BarChart();
            ~BarChart(){/*empty*/};
            /// Set the bar chart date.
            inline void set_date( std::string d );
            /// Add a single bar to the bar chart.
            inline void add( const string& label, value_t value, const string& category );
            /// Remove all bars from the chart.
            inline void clear();
            /// Sort the bars in descending order.
            inline void sort();

            //== Acessor methods.

            /// Retrives the bar chart date
            inline string get_date( void ) const { return m_date; }
            /// Retrives the value of the largest bar.
            inline value_t max_bar_value ( void );
            /// Returns true if the bar chart has no bars.
            inline bool empty( void ) const { return bars.empty(); }
            /// Returns the # of bars in the chart.
            inline size_t size( void ) const { return bars.size(); }
    };

} // namespace bcra.
#endif

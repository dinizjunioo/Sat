#include <algorithm>
using std::transform;
#include <string>
using std::string;
#include <sstream>
using std::istringstream;
#include <vector>

#include "bcr_am.h"
#include <iostream>
#include "../libs/coms.h"
#include "../libs/text_color.h"
#include <iomanip>  // centralizar strings
using std::setw;

#include <exception>
#include <thread>
#include <fstream>

namespace bcra
{
    // Instantiation of the global configuration variable.
    const Cfg global_cfg;

    auto STR_LOWERCASE = [](const char* t)->std::string {
        std::string str{ t };
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);
        return str;
    };

    /// Lambda expression that transform the string to lowercase.
    auto STR_UPPERCASE = [](const std::string& s)->std::string {
        std::string str{ s };
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);
        return str;
    };

    /// Tokenize a string
    /*!
     * @param input_str The input string.
     * @param delimiter The delimiter symbol.
     * @return A list of tokenized strings.
     */
    std::vector<std::string> split(const std::string& input_str, char delimiter)
    {
        // Store the tokens.
        // RETORNAR UMA STRUCT
        // COM ANO MES E OS CARALHO
        std::vector<std::string> tokens;

        // read file line by line
        std::istringstream iss;
        iss.str(input_str);
        std::string token;
        while (std::getline(iss >> std::ws, token, delimiter))
            tokens.push_back(token);

        return tokens;
    }



    /// Prints out usage instructions; any incoming string is treated as an error message.
    void BCRAnimation::usage(std::string msg = "") {
        if (msg != "") std::cerr << "ERR: " << msg << "\n\n";
        std::cerr << "Usage: bcr [<options>] <input_data_file>\n"
            << "  Bar Chart Race options:\n"
            << "      -b  <num> Max # of bars in a single char.\n"
            << "                Valid range is [1,15]. Default values is 5.\n"
            << "      -f  <num> Animation speed in fps (frames per second).\n"
            << "                Valid range is [1,24]. Default value is 24.\n";
        std::cerr << '\n';
        exit(msg != "" ? 1 : 0);
    }


    BCRAnimation::BCRAnimation()
    {
        m_opt.input_filename = "";
        m_opt.fps = global_cfg.default_fps;
        m_opt.n_bars = global_cfg.default_bars;
    }

    /// Initializes the animation engine.
    void BCRAnimation::initialize(int argc, char** argv)
    {
        // TODO: Process CLI here.

        // Traverse the list of incoming arguments sent via command line.
        
        for (auto i{ 0 }; i < argc; ++i) 
        {

            std::string param{ argv[i] }; // Convert current argumento into string form for convenience.
            // Let us handle each possible flag we may receive.
            if (param == "-b") { 
                // This flag receives an argument, so we need to check if one is available.
                if (i + 1 == argc) {
                    // No argument received, this means something is missing here....
                    usage("Faltou argumento para -b [5,15]");
                }
                // Conver string into the actual integer value.
                int nb{ 5 };
                try { nb = std::stoi(argv[++i]);}
                catch (const std::invalid_argument& e) {
                    usage("Qtd de barras invalida. Tente algo em [1,15].");
                }
                catch (const std::out_of_range& e) {
                    usage("Qtd de barras invalida. Valor numerico fora do aceitavel. Tente algo em [1,15].");
                }
                // Now we have a valid integer, we need to check if it lies in our desired range.
                if (nb < 1 || nb>15)
                    usage("Qtd de barras fora da faixa. Tente algo em [1,15].");
                // Finally we've got our valid # of bars from the user.
                m_opt.n_bars = nb;
            }
            else if (param == "-f") 
            {
                // TODO: process the flag in a way similar to '-b'.                
                int fps{12};
                try { fps = std::stoi(argv[++i]);}
                catch (const std::invalid_argument& e) {
                    usage("Qtd de barras invalida. Tente algo em [1,15].");
                }
                catch (const std::out_of_range& e) {
                    usage("Qtd de barras invalida. Valor numerico fora do aceitavel. Tente algo em [1,15].");
                }
                //  
                if (fps < 1 || fps >24)
                    usage("Frames por segundos fora da faixa. Tente algo entre o intervalo fechado [1,24].");
                // 
                m_opt.fps = fps;
                
            }

            else if (param == "-h" || param == "--help") {
                // Basic help here
                usage();
            }
            else {
            // This should be the **required* data file name.
            
                m_opt.input_filename = param;

                // Test if we have a valid file here.
                // Just try to open the input file and see if any error message comes out.
            }
        }

        // Set the initial animation state.
        m_animation_state = ani_state_e::START;

        std::ifstream file("data\\mossoro.txt"); // => data\\mossoro.txt m_opt.input_filename
        std::string str;

        if (!file.is_open()) {
            std::cout << "Saiu com erro! \n";    
            exit(EXIT_FAILURE);
        }

        std::string push_inicial_dates;

        getline(file, push_inicial_dates);
        m_barChart.main_title = push_inicial_dates;

        getline(file, push_inicial_dates);
        m_barChart.info_date = push_inicial_dates;

        getline(file, push_inicial_dates);
        m_barChart.fonte_date = push_inicial_dates;
        getline(file, push_inicial_dates);
        getline(file, push_inicial_dates);
        num_linha_por_blocos = stoi(push_inicial_dates);
        contador_charts = 0;

        while (getline(file, str))
        {
            // tentei o stoi mas nao deu muito certo => error: stoi what()
            if ((str.size() < 2))
            {

                contador_charts+=1;
                getline(file, str);
            }
            else
            {
                std::vector<std::string> aux;   //vector auxiliar

                aux = split(str, ',');

                m_barChart.bars.push_back ( BarChart::BarItem{ aux[1], stoi(aux[3]), aux[4] });

                for (auto i : aux)              // push back no vector aux
                    files.push_back(i);
        

                if (cores.size() == 0)
                {
                    //std::cout << "entrei " << std::endl;
                    cores.push_back(files.back());
                }
                else if (cores.size() >= 1)
                {
                    //std::cout << files.back() << std::endl;
                    if (test_cor(cores, files.back())) {cores.push_back(files.back());}
                }
            }
        }
        file.close();

        contador_charts = contador_charts; 
        
        // perceba que o contador na vdd soma dois no if
        // e estou somando mais 1 pois existe o ultimo else a ser lido, logo tem mais 1 chart.

         

        for (int i = 0; i < cores.size(); i++)
        {
            //std::cout << cores[i] << std::endl;
            //if (i >= 13)
              //  Pairs.insert(MID::value_type(Color::tcolor(cores[0], Color::color_list[0]), Color::tcolor("█", Color::color_list[0])));
            //else 
                Pairs.insert(std::pair<std::string, std::string>((Color::tcolor(cores[i], Color::color_list[i])), 
                                                            (Color::tcolor("█", Color::color_list[i]))));
            
        }
        
        

        indice_time = 0;
        count_value = 0;
        m_barChart.time_stamp = files[indice_time];
        
    }


    bool BCRAnimation::is_over(void) const
    {
        return m_animation_state == ani_state_e::END;
    }

    void BCRAnimation::process_events()
    {
        if (m_animation_state == ani_state_e::START)
        {
            //
        }
        else if (m_animation_state == ani_state_e::WELCOME)
        {
            press_enter();
        }
        else if (m_animation_state == ani_state_e::READING_INPUT)
        {
            // leitura dos dados (pode ser antes do welcome ?)

        }
        else if (m_animation_state == ani_state_e::RACING)
        {
          
        }
        else if (m_animation_state == ani_state_e::END)
        {
            // 
        }
        else if (m_animation_state == ani_state_e::ERROR)
        {
            // 
        }
    }

    void BCRAnimation::update()
    {
        if (m_animation_state == ani_state_e::START)
        {
            m_animation_state = ani_state_e::WELCOME;
        }
        else if (m_animation_state == ani_state_e::WELCOME)
        {
            // leitura de um enter ? talvez?!
            m_animation_state = ani_state_e::READING_INPUT;
        }
        else if (m_animation_state == ani_state_e::READING_INPUT)
        {
            // leitura dos dados (pode ser antes do welcome ?)
            m_animation_state = ani_state_e::RACING;
        }
        else if (m_animation_state == ani_state_e::RACING)
        {
            std::chrono::milliseconds  duration{ 1000 / m_opt.fps };
            std::this_thread::sleep_for(duration);

            if ((indice_time + (num_linha_por_blocos * 5))  < files.size())
            {
                indice_time +=  (num_linha_por_blocos * 5);
                count_value += m_opt.n_bars;
                m_barChart.time_stamp = files[indice_time];
            } else {
            m_animation_state = ani_state_e::END;
            }

            
        }
        else if (m_animation_state == ani_state_e::END)
        {
            // 
        }
        else if (m_animation_state == ani_state_e::ERROR)
        {
            // 
        }
    }

    void BCRAnimation::render(void) const
    {
        if (m_animation_state == ani_state_e::START)
        {
            //nao faz nada
        }
        else if (m_animation_state == ani_state_e::WELCOME)
        {
            print_welcome();
        }
        else if (m_animation_state == ani_state_e::RACING)
        {
            print_racing();
        }
        else if (m_animation_state == ani_state_e::END)
        {
            print_end();
        }
        else if (m_animation_state == ani_state_e::ERROR)
        {
            // 
        }
    }

    void BCRAnimation::print_racing(void) const
    {
        std::cout << Color::tcolor(m_barChart.main_title, Color::BLUE, Color::BOLD)  << std::endl;
        std::cout << std::endl;
        std::cout << "Time Stamp: " << Color::tcolor(m_barChart.time_stamp, Color::BLUE, Color::BOLD) << std::endl;

        //
        int j = 70; // é um ma
        MID::const_iterator iter2 = Pairs.begin();
        for (auto i{ 0 }; i < Pairs.size(); ++i)
        {

            for (auto k{ 0 }; k < j; ++k)
            {
                std::cout << iter2->second;
            }
            std::cout << " " << m_barChart.bars[i + count_value].category << "[" << m_barChart.bars[i + count_value].value << "]\n\n";
            iter2++;
            j -= 5;
        }
        std::cout << "\n";
        //
        std::cout << "+"; linha(90, '-'); std::cout << ">\n";
        std::cout << Color::tcolor(m_barChart.info_date, Color::BLUE, Color::BOLD) << "\n";
        std::cout << "\n\n";
        std::cout << Color::tcolor(m_barChart.fonte_date, Color::BLUE, Color::BOLD) << "\n";
        for (auto i : Pairs)
            std::cout << i.second << ": " << i.first << " ";
        std::cout << "\n\n";
       
    }

    void BCRAnimation::print_welcome(void) const
    {
        std::cout << "\n================================================\n";
        std::cout << "  Welcome to the Bar Char Race, v1.0\n";
        std::cout << "  Copyright (c) 2022, Diniz Rodrigues\n";
        std::cout << "================================================\n\n";
        coms::Message("Preparing to read input file " + m_opt.input_filename);
        std::cout << std::endl;
        coms::Message("Processing data, please wait.");
        coms::Message("Input file sucessfuly read.\n");
        coms::Message("We have \"" + std::to_string(contador_charts) + "\" Charts, each with \"" + std::to_string(m_opt.n_bars) + "\" bars\n");
        coms::Message("Animation speed is: " + std::to_string(m_opt.fps));
        coms::Message("Title: " + m_barChart.main_title);
        coms::Message("Value is: " + m_barChart.info_date);
        coms::Message("Source : " + m_barChart.fonte_date);
        coms::Message("# of categories found: " + std::to_string(cores.size()));
        coms::Message("Press enter to begin the animation.\n");

        
        for (auto i : Pairs)
            std::cout << i.first ; 

    }

    void BCRAnimation::print_end(void) const
    {
        std::cout << Color::tcolor(m_barChart.time_stamp, Color::BLUE, Color::BOLD) << "\n";
        std::cout << "\n\n\n\n";
        std::cout << Color::tcolor(m_barChart.info_date, Color::BLUE, Color::BOLD) << "\n";
        std::cout << "\n\n";
        std::cout << Color::tcolor(m_barChart.fonte_date, Color::BLUE, Color::BOLD) << "\n";
        for (auto i : Pairs)
            std::cout << i.second << ": " << i.first << " ";
        std::cout << "\n\n";
        std::cout << "Hope you have enjoyed the Bar Chart Race!\n";
    }
    
    bool BCRAnimation::search_binary(std::vector<std::string>::iterator it_init, std::vector<std::string>::iterator it_fim, const std::string word)
    {
        for (auto var = it_init; var != it_fim; var++)
        {
            if (*var == word)
                return true;
        }
        return false;
    }

    bool BCRAnimation::test_cor(const std::vector<std::string> test, const std::string word)
    {
        for(auto i {0}; i < test.size(); ++i)
        {
            if(test[i] == word)
                return false;
        }
        return true;
    }
    void BCRAnimation::press_enter(void)
    {
        //getchar();
        std::cin.ignore();
    }

    void BCRAnimation::linha(int a, char b) const
    {
        for (auto i = 0; i < a; i++)
        {
            std::cout << b;
        }
    }
};

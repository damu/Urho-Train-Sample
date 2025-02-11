#ifndef MISC_H
#define MISC_H

#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <map>
#include <chrono>
#include <iostream>

#include <Urho3D/Urho3D.h>
#include <Urho3D/Engine/Application.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Material.h>

// copied from my source collection.
/**
 * @brief The timer class can be used to measure times and optionaly output them automatically on destruction.
 * Example:
 * \code
 * {
 *   timer _("test");
 *   sleep(0.1);
 * }                        // the timer is destructed here as it goes out of scope and prints something like "0.100132 <- test"
 * \endcode
 */
class timer
{
public:
    std::string str;        ///< the message printed, after the measured time, on destruction
    std::chrono::steady_clock::time_point start;    ///< the time the timer was started on
    bool output;            ///< if the timer should print something on destruction

    timer(std::string str,bool output=true) : str(str),output(output){reset();}
    timer() : str(""),start(),output(false){reset();}
    ~timer()
    {
        if(!output)
            return;
        std::cout<<until_now()<<" <- "<<str<<std::endl;
    }

    /// resets the timer by putting the current time into the member /start/
    void reset(){start=std::chrono::steady_clock::now();}

    /// returns the time which has passed since starting the timer
    double until_now() const
    {
        std::chrono::steady_clock::time_point end=std::chrono::steady_clock::now();
        return(std::chrono::duration_cast<std::chrono::microseconds>(end-start).count()/1000000.0);
    }

    /// same as until_now()
    operator double() const {return until_now();}
};

/**
 * @brief This class takes lambdas and a delay (in seconds) to execute them later. The update() function is thought to be called each frame.
* Example:
 * \code
 * // add an action to be executed in 0.2s
 * delayed_actions.insert(0.2,
 *                        [this]
 *                        {
 *                            player_->light->SetBrightness(player_->light->GetBrightness()>0.5?0:1.5);  // toggle flashlight with a delay to fit to the sound
 *                        });
 *
 * \endcode
 */
class delayed_action_handler
{
    std::map<std::chrono::steady_clock::time_point,std::function<void()>> actions;
public:
    void insert(float wait_time,std::function<void()> lambda){actions.emplace(std::chrono::steady_clock::now()+std::chrono::milliseconds(int(wait_time*1000)),lambda);}

    void update()
    {
        auto time_over=std::chrono::steady_clock::now();
again:
        auto iter=actions.begin();
        if(iter!=actions.end())
            if(iter->first<=time_over)
            {
                iter->second();
                actions.erase(iter);
                goto again;
            }
    }
};

/// \brief Calls SetModel on the given model and tries to load the model file and all texture files mentioned in a model_name+".txt".
/// model_name is supposed to have no file extension. Example: "Data/Models/Box", loads the model "Data/Models/Box.mdl".
/// It's a template to support all model classes like AnimatedModel and StaticModel.
template<typename T>
void set_model(T* model,Urho3D::ResourceCache* cache,Urho3D::String model_name)
{
    Urho3D::String filename_model=model_name;
    model->SetModel(cache->GetResource<Urho3D::Model>(filename_model.Append(".mdl")));
    Urho3D::String filename_txt=model_name;
    filename_txt.Append(".txt");
    Urho3D::SharedPtr<Urho3D::File> file=cache->GetFile(filename_txt);

    if(!file->IsOpen())
        return;

    std::string str;
    str.resize(10000);
    str.resize(file->Read((void*)str.data(),(unsigned)str.size()));

    std::stringstream ss(str);
    std::string line;
    for(int i=0;getline(ss,line);i++)
    {
        if(line[line.size()-1]=='\r')
            line.resize(line.size()-1);
        model->SetMaterial(i,cache->GetResource<Urho3D::Material>(Urho3D::String(line.c_str(),line.size())));
    }
}

#endif // MISC_H

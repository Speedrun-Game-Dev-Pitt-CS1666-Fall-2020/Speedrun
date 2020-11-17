#include "XorShifter.h"
#include "MapGenerator.h"
#include "SimplexNoise.h"
#include <cmath>
#include <vector>
#include <iostream>
#include <SDL.h>
#include <cfloat>

#define TAU 6.283185307f
#define PI 3.141592654f
#define TUBE_LENGTH 200
#define TUBE_LENGTH_VARY_AMOUNT 1
#define INFLUENCE 1.f//scale of 1-2
#define ANTI_FLUENCE 0.f
#define STRENGTH 3.0f
#define MAX_CAVE_WIDTH 5.f
#define MIN_CAVE_WIDTH 1.f
#define DENSITY_THRESH 100

#define ARBITRARY_CONSTANT 0.f

MapGenerator::MapGenerator(GameMap* map){
    this->map = map;
    this->rng = new XorShifter(map->seed);

    this->genTube();
    this->voxelize();
    this->genBiomes();
}

MapGenerator::~MapGenerator(){
    delete tubePoints;
    delete tubeWidths;
}

void MapGenerator::genBiomes(){
    SimplexNoise* sliminess = new SimplexNoise(this->rng->next());
    SimplexNoise* temperature = new SimplexNoise(this->rng->next());
    SimplexNoise* obstacle = new SimplexNoise(this->rng->next());

    sliminess->freq = 0.01f;
    temperature->freq = 0.01f;
    obstacle->freq = 0.1f;
    sliminess->octaves = 3.f;
    temperature->octaves = 3.f;
    obstacle->octaves = 3.f;
    sliminess->updateFractalBounds();
    temperature->updateFractalBounds();
    obstacle->updateFractalBounds();

    int w = this->map->w;
    int h = this->map->h;
    map->mat_field = new unsigned char*[w];
    for(int x = 0; x < w; x++){
        unsigned char* col = new unsigned char[h];
        map->mat_field[x] = col;
        for(int y = 0; y < h; y++){
            unsigned char mat = 0;
            
            float obs = obstacle->getSingle(x,y);
            if(obs > 0.5f){
                float slime = sliminess->getSingle(x,y);
                float temp = temperature->getSingle(x,y);

                float thresh = 0.75f*temp+0.15f;
                

                if(slime > thresh){//ice, bouncy slime, or sludge
                    if(temp > 0.8){
                        mat = 2;//change to dangerous slime later
                    }else if(temp > 0.4){
                        mat = 2;
                    }else{
                        mat = 1;
                    }
                }else{
                    if(temp > 0.8){
                        mat = 0;//add lava biome later
                    }else{
                        mat = 0;
                    }
                }
            }
            


            this->map->mat_field[x][y] = mat;
        }
    }

    delete sliminess;
    delete temperature;
    delete obstacle;
}

void MapGenerator::voxelize(){
    tube_min.x = floorf(tube_min.x-MAX_CAVE_WIDTH-4.f);
    tube_min.y = floorf(tube_min.y-MAX_CAVE_WIDTH-4.f);
    tube_max.x = ceilf(tube_max.x+MAX_CAVE_WIDTH+4.f);
    tube_max.y = ceilf(tube_max.y+MAX_CAVE_WIDTH+4.f);

    const int w = tube_max.x-tube_min.x;
    const int h = tube_max.y-tube_min.y;
    map->w = w;
    map->h = h;

    //translate all tube points into respect of the index field minimums
    for(int i = 0; i < tubeLength; i++){
        tubePoints[i] -= tube_min;
    }

    SimplexNoise* noise = new SimplexNoise(this->rng->next());
    noise->octaves = 2;
    noise->freq = 0.1f;
    noise->updateFractalBounds();

    //initialize scalar field
    map->field = new unsigned char*[w];
    for(int x = 0; x < w; x++){
        unsigned char* col = new unsigned char[h];
        map->field[x] = col;
        for(int y = 0; y < h; y++){

            Vec2 pos = Vec2(x,y);
            float density = 0xFF;
            //populate scalar field:
            
            //distance to path
            Vec2 v;
            float vr;
            Vec2 w = tubePoints[0];
            float wr = tubeWidths[0];
            float min_density = 1.f;
            for(int i = 1; i < tubeLength; i++){
                v = w;
                w = tubePoints[i];
                vr = wr;
                wr = tubeWidths[i];

                float vmin = v.y - vr;
                float vmax = v.y + vr;
                float wmin = w.y - wr;
                float wmax = w.y + wr;
                float bmin = vmin < wmin ? vmin : wmin;
                float bmax = vmax > wmax ? vmax : wmax;
                if(
                    pos.y < bmin || pos.y > bmax
                )continue;//only y bounding box check, holy moly its fast anyway imma keep it tho

                vmin = v.x - vr;
                vmax = v.x + vr;
                wmin = w.x - wr;
                wmax = w.x + wr;
                bmin = vmin < wmin ? vmin : wmin;
                bmax = vmax > wmax ? vmax : wmax;
                if(
                    pos.x < bmin || pos.x > bmax
                )continue;//only y bounding box check, holy moly its fast anyway imma keep it tho
                
                //let v be origin
                Vec2 line = w-v;
                Vec2 tpos = pos-v;
                float line_sqrmag = line.sqrMag();
                float line_mag = sqrtf(line_sqrmag);

                float norm_comp = Vec2::dot(line,tpos)/line_sqrmag;
                
                if(norm_comp < 0){
                    float d = tpos.sqrMag()/(vr*vr);
                    min_density = d < min_density ? d : min_density;
                }else if(norm_comp < 1){
                    //std::cout << norm_comp << "\n";
                    float r = vr*(1-norm_comp)+wr*norm_comp;
                    Vec2 proj = line*norm_comp;
                    //std::cout << line.mag() << " " << proj.mag() << "\n";
                    float sqrDist = (tpos - proj).sqrMag();
                    
                    //std::cout << tpos.x<<","<<tpos.y << " - " << proj.x<<","<<proj.y << "\n";
                    float d = sqrDist/(r*r);
                    min_density = d < min_density ? d : min_density;
                }
            }

            //roughness
            min_density -= (noise->getFractal(pos.x,pos.y)*0.9f);//change to += later


            min_density *= 256.f;
            if(min_density < 0.f)min_density = 0.f;
            else if(min_density >= 256.f)min_density = 255.f;
            col[y] = (unsigned char)(min_density);
        }
    }
}

//-1 to 1 lerp
inline float lerp_theta(float s){
    float sign = s > 0.f ? 1.f : -1.f;
    s*=sign;

    return sign * (s > 0.5f ? 1.f-sqrtf(0.5f-0.5f*s) : sqrtf(s*0.5f));    
}
void MapGenerator::genTube(){
    Vec2 pos = Vec2(0.f,0.f);
    //Vec2 vel = Vec2(1.f,0.f);


    this->tubeLength = TUBE_LENGTH+(this->rng->next()%TUBE_LENGTH_VARY_AMOUNT);
    this->tubePoints = new Vec2[this->tubeLength];
    this->tubeWidths = new float[this->tubeLength];

    float t = 500.f;

    SimplexNoise* wormy = new SimplexNoise(rng->next());

    //used for calculating variance of simplex noise! ANSWER: 0.0484
    /*
    double variance = 0.0;
    for(int i = 0; i < 10000000; i++){
        double val = (double)(wormy->getFractal((float)i,420.f)-0.5f);
        variance += val*val;
    }
    variance /= (double)(10000000-1);
    std::cout << "variance: " << variance << '\n';
    */

    SimplexNoise* noise_w = new SimplexNoise(rng->next());

    wormy->octaves = 3;
    wormy->updateFractalBounds();

    noise_w->octaves = 1;
    noise_w->freq = 0.1f;
    noise_w->updateFractalBounds();

    Vec2 min = Vec2(FLT_MAX,FLT_MAX);
    Vec2 max = Vec2(FLT_MIN,FLT_MIN);
    //float avg = 0;
    for(int i = 0; i < tubeLength; i++){

        min.x = min.x > pos.x ? pos.x : min.x;
        min.y = min.y > pos.y ? pos.y : min.y;
        max.x = max.x < pos.x ? pos.x : max.x;
        max.y = max.y < pos.y ? pos.y : max.y;

        this->tubePoints[i] = pos;
        this->tubeWidths[i] = noise_w->getFractal(pos.x+100.f, pos.y)*MAX_CAVE_WIDTH+MIN_CAVE_WIDTH;        

        float theta = wormy->getFractal(t,420.f);
        theta = erff32((theta-0.5f)/(0.3111269f));//(-> erf((x-mu) / sqrt(2*variance)) ) maps the simplex noise output that has a normal distribution (variance=~0.0484) to the error function to get a more uniform distribution
        
        theta = lerp_theta(theta);
        //theta = theta > 0 ? sqrtf(theta*0.25f+0.25f) : 1.f-(sqrtf(-1.f*theta)*0.5f);//further map to distribute less down and more across using fast piecewise parabolas
        //theta*=2.f;
        //theta-=1.f;
        
        theta *= 0.7f;
        theta *= PI;

        //theta = theta < 0 ? TAU+theta : theta;
        //theta is now 0-TAU
        //theta *= 0.75f;
        //theta -= PI/4.f;
        //theta = theta < 0 ? TAU+theta : theta;
        //std::cout << theta << '\n';
        t+=8.f;
        //t+=STRENGTH;

        //Vec2 velFrame = Vec2(dx,dy).normal();
        //vel = (vel*ANTI_FLUENCE + velFrame*INFLUENCE).normal();
        //Vec2 v = vel*STRENGTH;
        Vec2 v = Vec2(sinf(theta)*1.5f,cosf(theta))*STRENGTH;
        pos += v;
        //pos.y += 0.5f;
        //std::cout << velRad << '\n'
        //    << std::cos(velRad) << '\n';
    }
    /*
    SimplexNoise* simplex_noise = nullptr;

    Vec2 pos = Vec2(0.f,0.f);
    for(int i = 0; i < tubeLength; i++){
        this->tubePoints[i] = pos;

        float theta = simplex_noise->getFractal(t, ARBITRARY_CONSTANT);
        
        theta *= 0.7f;
        theta *= PI;

        t+=8.f;

        Vec2 v = Vec2(sinf(theta)*1.5f,cosf(theta))*STRENGTH;
        pos += v;
    }
    */

    //std::cout << "min: " << min << '\n'
    //    << "max: " << max << '\n';
    this->tube_min = min;
    this->tube_max = max;
    delete wormy;
    delete noise_w;
}

float bias_mapping(float f){
    return f;
}

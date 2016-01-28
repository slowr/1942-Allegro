#include "AnimationFilmHolder.h"

AnimationFilmHolder AnimationFilmHolder::holder;

void AnimationFilmHolder::Load(const std::string& catalogue) {
		std::ifstream fin;
		fin.open(catalogue);
		assert(fin.good());

		do {
			if (fin.eof()) break;
			char *path;
			int numBitmaps;
			{
				/* 
					Read the path of sprite sheet and the number of total bitmaps inside
				*/
				char buf[MAX_CHARS_PER_LINE];
				fin.getline(buf, MAX_CHARS_PER_LINE);

				path = strtok(buf, DELIMITER);
				numBitmaps = atoi(strtok(0, DELIMITER));
			}

			ALLEGRO_BITMAP * bmp = bitmaps.Load(path);
			al_convert_mask_to_alpha(bmp, al_map_rgb(0, 67, 171));

			/*
				For each bitmap read the id, number of frames and construct the rectangles
			*/
			for (int bitmapNo = 0; bitmapNo < numBitmaps; bitmapNo++){
				char * id;
				int numFrames;
				std::vector<Rect> rects;
				{
					char buf[MAX_CHARS_PER_LINE];
					fin.getline(buf, MAX_CHARS_PER_LINE);

					id = strtok(buf, DELIMITER);
					numFrames = atoi(strtok(0, DELIMITER));
				}
				{
					for (int i = 0; i < numFrames; i++){
						char buf[MAX_CHARS_PER_LINE];
						int x, y, w, h;
						fin.getline(buf, MAX_CHARS_PER_LINE);
						x = atoi(strtok(buf, DELIMITER));
						y = atoi(strtok(0, DELIMITER));
						w = atoi(strtok(0, DELIMITER));
						h = atoi(strtok(0, DELIMITER));
						rects.push_back(Rect(Point(x, y), w, h));

						/*
							Calculate collision mask for pixel perfect collisions
							*/
						//{
						//	CollisionMask * collision_mask = (CollisionMask *)calloc(h * w / MASKDEPTH, sizeof(CollisionMask));
						//	std::ostringstream oss;
						//	float a, t;
						//	for (int curr_y = y, counter = 0; curr_y < y + h; curr_y++){
						//		for (int curr_x = x; curr_x < x + w; curr_x++, counter++){
						//			ALLEGRO_COLOR c = al_get_pixel(bmp, curr_x, curr_y);
						//			al_unmap_rgba_f(c, &t, &t, &t, &a);
						//			if (a != 0){
						//				collision_mask[counter / MASKDEPTH] |= 1 << ((MASKDEPTH - counter - 1) % MASKDEPTH);
						//			}
						//		}
						//	}

						//	oss << i;
						//	for (int k = 0; k < w*h / MASKDEPTH; k++){
						//		if ( (k*MASKDEPTH) % w == 0) oss << std::endl;
						//		/*std::bitset<MASKDEPTH> x(collision_mask[k]);
						//		oss << x;*/
						//		oss << collision_mask[k] << " ";
						//	}

						//	std::ofstream outFile;
						//	std::string name = "resources/";
						//	name.append(id);
						//	name.append(".collision");
						//	outFile.open(name.c_str(), std::ios_base::app);
						//	outFile << oss.str() << std::endl;
						//	outFile.close();
						//}
					}
				}
				films[id] = new AnimationFilm(bmp, rects, id);
			}
		} while (true);
	}

void AnimationFilmHolder::CleanUp(void) {
		for (Films::iterator i = films.begin(); i != films.end(); ++i)
			delete(i->second);
		films.clear();
	}

AnimationFilm* AnimationFilmHolder::GetFilm(const std::string id) const {
		Films::const_iterator i = films.find(id);
		return i != films.end() ? i->second : (AnimationFilm*)0;
	}

AnimationFilmHolder& AnimationFilmHolder::Get(void) { return holder; }

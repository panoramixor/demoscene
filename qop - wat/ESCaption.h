#ifndef CAPTION_H
#define CAPTION_H

class CCaption
{
	public:
			CCaption();
			~CCaption();
//			void Create(float time1, float x1, float y1, float z1, char *line1, float time2, float x2, float y2, float z2, char *line2, float time3, float x3, float y3, float z3, char *line3, float time4, float x4, float y4, float z4, char *line4, float time5, float x5, float y5, float z5, char *line5);
			void Create(float time1, float x1, float y1, float z1, char *line1, float time2, float x2, float y2, float z2, char *line2, float time3, float x3, float y3, float z3, char *line3, float time4, float x4, float y4, float z4, char *line4, float time5, float x5, float y5, float z5, char *line5, float timelenght);
			void Draw(float scenetime);
			void Draw(float scenetime, int effect);
			void Load();
	private:
			void Sequence(KeyFrame SceneKeyFrames[], int MAX_KEYFRAMER_INDEX, int effect, float scenetime);
			void Print(float x, float y, float z, char *string);
			int keyframer_index;	
			unsigned int strsize;
			BFL_FONT  *bflfont;
			FILE *fontfile;
			KeyFrame kf[4*5];
			KeyFrame k_caption;
};

#endif /* CAPTION_H */
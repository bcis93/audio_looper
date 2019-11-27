class Track
{
public:
	Track();
	Track(int id);
	~Track();
	bool isPlaying();
	bool isRecording();
	void startPlaying();
	void startRecording();
	void stopPlaying();
	void stopRecording();
private:
	bool playing;
	bool recording;
	int id;
};


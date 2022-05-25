#include "pch.h"
class PerformanceMeasure
{
public:
	PerformanceMeasure(std::string description)		
	{
		m_Description = description;
		m_StartTimepoint = std::chrono::high_resolution_clock::now();
	}

	~PerformanceMeasure()
	{
		Stop();
	}

	void Stop()
	{
		auto endTimepoint = std::chrono::high_resolution_clock::now();
		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
		auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		auto duration = end - start;
		double sec = duration * 0.001 * 0.001;
		std::cout << m_Description << ": " << duration << "us (" << sec << "sec)\n";
	}
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
	std::string m_Description;
};


#pragma once

#include "core/SimulationObject.hpp"

class SignalDisplayObject : public SimulationObject
{
public:
    SignalDisplayObject(int size) : size(size), m_SignalBuffer(size, 0)
    {
    }

    void Initialize() override
    {
        m_SignalBuffer.resize(size, 0);
    }

    void Update(double dt) override
    {
        time += static_cast<float>(dt);
        addValue(generateSineWave(time));
    }

    void Render() override
    {
        if (ImGui::Begin("Signal Display"))
        {
            if (ImPlot::BeginPlot("Signal Plot"))
            {
                ImPlot::SetupAxis(ImAxis_X1, "Count");
                // ImPlot::SetupAxisFormat(ImAxis_X1, "%d Count");

                ImPlot::SetupAxis(ImAxis_Y1, "Amplitude");
                ImPlot::SetupAxisFormat(ImAxis_Y1, "%0.1f V");

                ImPlot::PlotLine("Signal", m_SignalBuffer.data(), (int)m_SignalBuffer.size());
            }
            ImPlot::EndPlot();
        }
        ImGui::End();
    }

    void
    Finalize() override
    {
        m_SignalBuffer.resize(size, 0);
    }

    void Reset() override
    {
        m_SignalBuffer.resize(size, 0);
        index = 0;
    }

    float generateSineWave(float time)
    {
        float amplitude = 10;
        float frequency = 1000;
        return amplitude * static_cast<float>(sin(frequency * time));
    };

    void addValue(float value)
    {
        // if index is greater than the size of the signal, reset the index
        if (index >= m_SignalBuffer.size())
        {
            index = 0;
        }

        m_SignalBuffer[index] = value;
        index++;
    }

    const std::vector<float> &getSignal() const
    {
        return m_SignalBuffer;
    }

private:
    std::vector<float> m_SignalBuffer;
    float time = 0;
    int size = 0;
    int index = 0;
};
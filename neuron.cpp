#include "neuron.h"
#include "connection.h"
#include <cstdlib>
#include <cmath>


Neuron::Neuron(unsigned numOutputs, unsigned myIndex)
{

    for (unsigned c = 0; c < numOutputs; ++c) {
        m_outputWeights.push_back(Connection());
        m_outputWeights.back().weight = randomWeight();
    }

    m_myIndex = myIndex;

}

double Neuron::lr = 0.5;

void Neuron::feedForward(const Layer &prevLayer) {
    double sum = 0.0;

    // sum previous layer's outputs to use as inputs
    // include the bias node from the previous layer

    for (unsigned neuronNum = 0; neuronNum < prevLayer.size(); ++neuronNum) { // includes bias neuron
        sum += prevLayer[neuronNum].getOutputVal() *
                prevLayer[neuronNum].m_outputWeights[m_myIndex].weight;
    }

    m_outputVal = Neuron::activationFunction(sum);
}

void Neuron::updateInputWeights(Layer& prevLayer) {
    for (unsigned neuronNum = 0; neuronNum < prevLayer.size(); ++neuronNum) {
        Neuron &neuron = prevLayer[neuronNum];
        double newDeltaWeight = lr * neuron.getOutputVal() * m_gradient;

        neuron.m_outputWeights[m_myIndex].deltaWeight = newDeltaWeight;
        neuron.m_outputWeights[m_myIndex].weight += newDeltaWeight;
    }
}

void Neuron::calcOutputGradients(double targetVal) {
    double delta = targetVal - m_outputVal;
    m_gradient = delta * activationFunctionDerivative(m_outputVal);
}

void Neuron::calcHiddenGradients(const Layer& nextLayer) {
    double derivativeOfWeights = sumDOW(nextLayer);
    m_gradient = derivativeOfWeights * activationFunctionDerivative(m_outputVal);
}

double Neuron::sumDOW(const Layer& nextLayer) const {
    double sum = 0.0;

    for (unsigned neuronNum = 0; neuronNum < nextLayer.size() - 1; ++neuronNum) {
        sum += m_outputWeights[neuronNum].weight * nextLayer[neuronNum].m_gradient;
    }
    return sum;

}

double Neuron::activationFunction(double x) {
    // tanh - output range [-1 ... +1]
    return tanh(x);
//    return 1 / (1-exp(-x));
}


double Neuron::activationFunctionDerivative(double x) {
    // tanh derivative
    return 1 - x * x;
    // sigmoid deriv
//    return activationFunction(x) * (1 - activationFunction(x));

}

double Neuron::randomWeight() {
    return rand() / double(RAND_MAX);
}

void Neuron::setOutputVal(double val) {
    m_outputVal = val;
}

double Neuron::getOutputVal() const{
    return m_outputVal;
}
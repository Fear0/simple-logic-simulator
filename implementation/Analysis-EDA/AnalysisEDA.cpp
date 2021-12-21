  /*
 * AnalysisEDA.cpp
 *
 * This file contains the implementation of the simulator.
 */

#include "AnalysisEDA.h"
#include "Graph/GraphHandler.h"
#include <iostream>
#include <algorithm>
#include <regex>
#include <string.h>



void AnalysisEDA::run() {

    auto nets = graphHandler->getAllNets();
    auto elements = graphHandler->getAllElements();

    // sort the elements by their number to compute logic in the right order
    std::sort(elements.begin(),elements.end(),[](const auto& a, const auto&b){
        
        std::string s1 = a->getName().substr(1,a->getName().length()-1);
        std::string s2 = b->getName().substr(1,b->getName().length()-1);
        return std::stoi(s1) < std::stoi(s2);
    } );
   
    size_t numberNets = nets.size();
    std::vector<Logic> netsOutputs;

    // initialize the logic values of the nets to logicX
    for (size_t i = 0;  i < numberNets; i++){
        netsOutputs.emplace_back(Logic::logicX);
    } 
   
    // iterate over each inputvector where each inputvector represents 1 tact.
    for (const auto& inputVector : inputData){

        std::vector<Logic> outputs;
        size_t index = 0;
   
        std::vector<const Element*> flipflops;
        // get the flipflops
        for (const Element* element : elements){
            if (element->getElementInfo()->getType() == ElementType::Dff){
                flipflops.push_back(element);
            } 
        } 
        // pass the input terminal values of the flipflops to the corresponding outnets.
        for (const Element* element : flipflops){
            auto inNets = element->getInNets();
            auto outNets = element->getOutNets();
            const Net* inNet2 = inNets.at(1);
            size_t inNet2ID = inNet2->getId();
            Logic in2Value = netsOutputs.at(inNet2ID);
            auto outNet = outNets.at(0);
            size_t outNetID = outNet->getId();
            std::swap(netsOutputs.at(outNetID),in2Value);  
        }

        // Assign the input values to their corresponding nets
        for (size_t i = 0; i < numberNets ; i++){
            if (nets[i]->getInElement() == nullptr){
                Logic input = inputVector.at(index);
                auto itPos = netsOutputs.begin() + i;
                netsOutputs.erase(itPos);
                netsOutputs.insert(itPos,input);
                index++;
            } 
        } 

        // iterate over all combinatorial elements and compute their output values depending on the inputs.
        for (const Element* element : elements){
            if (element->getElementInfo()->getType() != ElementType::Dff){
            ElementType type = element->getElementInfo()->getType();
            auto inNets = element->getInNets();
            size_t size = inNets.size();
            auto outNets = element->getOutNets();
            const Net* inNet1 = inNets.at(0);
            const Net* inNet2;
            size_t inNet2ID;
            auto outNet = outNets.at(0);
            size_t outNetID = outNet->getId();
            size_t inNet1ID = inNet1->getId();
            Logic in2Value;
            if (size > 1){
                inNet2 = inNets.at(1);
                inNet2ID = inNet2->getId();
                in2Value = netsOutputs.at(inNet2ID);
            } 
            Logic in1Value = netsOutputs.at(inNet1ID);
            Logic outValue;
            switch (type){
                case ElementType::Not:
                    if (in1Value == Logic::logicX){
                        outValue = Logic::logicX;
                    } 
                    if (in1Value == Logic::logic1){
                        outValue = Logic::logic0;
                    } 
                    if (in1Value == Logic::logic0){
                        outValue = Logic::logic1;
                    } 
                    break;
                case ElementType::Or:
                    if (in1Value == Logic::logic1 || in2Value == Logic::logic1){
                        outValue = Logic::logic1;
                    } else if ((in1Value == Logic::logicX && in2Value == Logic::logicX)|| (in2Value == Logic::logicX && in1Value == Logic::logicX) ){
                        outValue = Logic::logicX;
                    } else if ((in1Value == Logic::logicX && in2Value == Logic::logic0)|| (in2Value == Logic::logicX && in1Value == Logic::logic0) ){
                        outValue = Logic::logicX;
                    } else if (in1Value == Logic::logic0 && in2Value == Logic::logic0){
                        outValue = Logic::logic0;
                    } 
                    break;
                case ElementType::And:
                    if (in1Value == Logic::logic0 || in2Value == Logic::logic0){
                        outValue = Logic::logic0;
                    } else if (in1Value == Logic::logicX|| in2Value == Logic::logicX ){
                        outValue = Logic::logicX;
                    } else if ((in1Value == Logic::logic1 && in2Value == Logic::logic1)){
                        outValue = Logic::logic1;
                    } 
                    break;
                default:
                    break;
            } 

            // update each outputvalue
            std::swap(netsOutputs.at(outNetID),outValue);
            }
            }

    
        std::cout << std::endl;
        // get the primary outputs
        for (const Net* net : nets){
            if (net->getOutElements()[0] == nullptr){
                size_t netId = net->getId();
                outputs.push_back(netsOutputs.at(netId));
            }  
        } 

        // print primary outputs for each input vector
        for (size_t i = 0; i < outputs.size(); i++){
            std::cout << outputs.at(i);
            if (i != outputs.size()-1){
                std::cout << ";";
            } 
        } 
        std::cout << std::endl;
    }

    // Todo: Implement the simulator here

#if false
    /*
     * The following code shows some exemplary usage of the API
     */

    // Iterate all elements:
    for (const Element* element : graphHandler->getAllElements()) {
        std::cout << element->getName() << std::endl;
    }

    // Iterate all nets:
    for(const Net* net: graphHandler->getAllNets()) {
        std::cout << net->getName();
        if (net->getInElement() == nullptr)
            std::cout << " (primary input)";
        if (net->getOutElements()[0] == nullptr)
            std::cout << " (primary output)";
        std::cout << std::endl;
    }

    // Iterate all time steps:
    for (const std::vector<Logic>& timeStep : inputData) {
             for (const Logic& value : timeStep) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
#endif
}

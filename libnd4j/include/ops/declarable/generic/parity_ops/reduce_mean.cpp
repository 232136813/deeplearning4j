//
// @author Yurii Shyrma (iuriish@yahoo.com), created on 01.06.2018
//


#include <ops/declarable/CustomOperations.h>


namespace nd4j    {
namespace ops     {

//////////////////////////////////////////////////////////////////////////
CUSTOM_OP_IMPL(reduce_mean, 1, 1, false, 0, 0) {

    NDArray<T> *input   = INPUT_VARIABLE(0);     

    NDArray<T> *output  = OUTPUT_VARIABLE(0);

    const bool keepDims = block.getTArguments()->size() > 0 ? (bool)T_ARG(0) : false;
    
    std::vector<int> dimensions = *block.getIArguments();    

    REQUIRE_TRUE(dimensions.size() <= input->rankOf(), 0, "REDUCE_MEAN OP: the number of dimensions to reduce along must be <= input array rank, but got %i instead" , dimensions.size());

    for(const auto& item : dimensions)
        REQUIRE_TRUE(item > -input->rankOf() || item < input->rankOf(), 0, "REDUCE_MEAN OP: the input dimension to reduce along must be in range (-%i, %i), but got %i instead !" , input->rankOf(), input->rankOf(), item);
    
    input->template reduceAlongDimension<simdOps::Mean<T>>(output, dimensions, keepDims);    

    return Status::OK();
}


DECLARE_SHAPE_FN(reduce_mean) {    

    const bool keepDims = block.getTArguments()->size() > 0 ? (bool)T_ARG(0) : false;
    
    std::vector<int> dimensions = *block.getIArguments();

    REQUIRE_TRUE(dimensions.size() <= inputShape->at(0)[0], 0, "REDUCE_MEAN OP: the number of dimensions to reduce along must be <= input array rank, but got %i instead" , dimensions.size());
    
    for(const auto& item : dimensions)
        REQUIRE_TRUE(item > -inputShape->at(0)[0] || item < inputShape->at(0)[0], 0, "REDUCE_MEAN OP: the input dimension to reduce along must be in range (-%i, %i), but got %i instead !" , inputShape->at(0)[0], inputShape->at(0)[0], item);

    Nd4jLong* outShapeInfo = ShapeUtils<T>::evalReduceShapeInfo(shape::order(inputShape->at(0)), dimensions, inputShape->at(0), keepDims, false, block.getWorkspace());

    return SHAPELIST(outShapeInfo);
}




}
}

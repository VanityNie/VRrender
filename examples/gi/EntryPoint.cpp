//
// Created by win111 on 2024/4/29.
//


#include "RHI/Windows.h"
#include "RHI/VulkanContext.h"
#include"RHI/Device.h"
#include"shaderc/shaderc.hpp"
#include<map>
#include<regex>

#include<iostream>


std::map<std::string, std::string> ParseSpecificFields(std::string input) {
    std::regex field_regex(R"((HW Unit|MetricType|RollupOp|Submetric|DimUnit):\s*<em>([^<]*)</em>)");
    std::smatch matches;
    std::map<std::string, std::string> fields;


    auto ReplaceCommas = [&](const std::string& str)->std::string {
        std::string result = str;
        std::replace(result.begin(), result.end(), ',', '_'); // 将逗号替换为下划线
        return result;

     };


    std::regex intro_regex(R"((.*?)<br/>HW Unit: <em>(.*?)</em>)");
    if (std::regex_search(input, matches, intro_regex)) {
        fields["Intro"] = ReplaceCommas( matches[1].str());
    }

    // 使用正则表达式搜索并拆分字符串
    while (std::regex_search(input, matches, field_regex)) {
        fields[matches[1].str()] = matches[2].str();
        input = matches.suffix().str(); // 更新剩余的字符串
    }

    // 确保所有指定的字段都被添加到映射中，如果没有找到则添加空字符串
    std::vector<std::string> specificFields = { "HW Unit", "MetricType", "RollupOp", "Submetric", "DimUnit" };
    for (const auto& field : specificFields) {
        if (fields.find(field) == fields.end()) {
            fields[field] = "";
        }
    }

    

    return fields;
}


int main()
{

    shaderc::CompileOptions;
    
    std::string input = "cumulative # of CTAs in flight, launched from the direct queue<br/>HW Unit: <em>tpc</em><br/>MetricType: <em>Counter</em><br/>RollupOp: <em>.avg</em><br/>Submetric: <em>.peak_sustained_elapsed</em><br/>DimUnit: <em>(ctas * gpc_cycles)</em>"
        ;
 

    // 调用函数来解析字段并获取映射
    std::map<std::string, std::string> fields = ParseSpecificFields(input);

    // 打印所有字段和值
    for (const auto& field : fields) {
        std::cout << field.first << ": " << field.second << std::endl;
    }
    

    std::cout << fields["HW Unit"];

  


    //Logger::init();
    //Windows windows;
    //VulkanContext vulkan_context{&windows,true};
    //Device device(&vulkan_context);
    //device.create_device();
    //
    
 /*   windows.loop();

    LOG_INFO("over");*/
    return 0;
}
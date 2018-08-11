#include "model.h"
#include "display.h"
#include "intervalScanLine.h"
#include "shader.h"
#include "def.h"

int main(int argc, char **argv)
{
	//加载模型
	Model model("../models/dolphins.obj");

	//着色
	Shader shader;
	//点光源
	shader.setLightColor(Color3f(0.3f, 0.3f, 0.3f));
	shader.setLightPosition(Point3f(400.0f, 600.0f, 500.0f));
	//环境光
	shader.setAmbientColor(Color3f(0.3f, 0.3f, 0.3f));
	

	//运行区间扫描线
	IntervalScanLine scanLine(WIDTH, HEIGHT);
	scanLine.run(model, shader);

	//显示
	Display dispaly("intervalScanLine",&model, &scanLine, &shader);
	dispaly.run(argc, argv);

	return 0;
}
#include "model.h"
#include "display.h"
#include "intervalScanLine.h"
#include "shader.h"
#include "def.h"

int main(int argc, char **argv)
{
	//����ģ��
	Model model("../models/dolphins.obj");

	//��ɫ
	Shader shader;
	//���Դ
	shader.setLightColor(Color3f(0.3f, 0.3f, 0.3f));
	shader.setLightPosition(Point3f(400.0f, 600.0f, 500.0f));
	//������
	shader.setAmbientColor(Color3f(0.3f, 0.3f, 0.3f));
	

	//��������ɨ����
	IntervalScanLine scanLine(WIDTH, HEIGHT);
	scanLine.run(model, shader);

	//��ʾ
	Display dispaly("intervalScanLine",&model, &scanLine, &shader);
	dispaly.run(argc, argv);

	return 0;
}
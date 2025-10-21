#include "vtkActor.h"
#include "vtkDataSetMapper.h"
#include "vtkFloatArray.h"
#include "vtkPointData.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkSmartPointer.h"
#include "vtkStructuredPoints.h"
#include "vtkAutoInit.h"

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)

int main()
{
	srand(0);

	vtkSmartPointer<vtkStructuredPoints> SP =
		vtkSmartPointer<vtkStructuredPoints>::New();
	SP->SetOrigin(0, 0, 0); // Начало координат
	SP->SetDimensions(10, 10, 10); // Размер сетки
	SP->SetSpacing(10, 10, 10); // Размер между узлами сетки - x,y,z

	vtkSmartPointer<vtkFloatArray> FA =
		vtkSmartPointer<vtkFloatArray>::New();
	for (int i = 0; i < 10 * 10 * 10; i++) {
		FA->InsertValue(i, i * 0.02); // Заполняем массив значениями
		//FA->InsertValue(i, (float)rand() / RAND_MAX); // Заполняем массив значениями
	}
	SP->GetPointData()->SetScalars(FA); // Привязка данных к сетке

	vtkSmartPointer<vtkDataSetMapper> mapper =
		vtkSmartPointer<vtkDataSetMapper>::New();
	mapper->SetInputData(SP);
	mapper->SetScalarRange(0, 20); // диапазон значений для выбора цвета, 0 - красный, 20 - синий

	vtkSmartPointer<vtkActor> actor =
		vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);

	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(actor);

	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);

	vtkSmartPointer<vtkRenderWindowInteractor> interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(renderWindow);

	renderWindow->Render();
	interactor->Start();

	return 0;
}
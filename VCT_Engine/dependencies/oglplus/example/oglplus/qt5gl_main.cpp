/*
 *  .file example/oglplus/qt5gl_main.cpp
 *  Implements Qt5-GL-based program main function for running examples
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <oglplus/gl.hpp>
#include <oglplus/config/fix_gl_version.hpp>
#include <oglplus/config/fix_gl_extension.hpp>

#include "example.hpp"
#include "example_main.hpp"

#include <QtGui>
#include <QApplication>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QStatusBar>
#include <QLabel>

#include "qt5gl_main.hpp"

#include <vector>
#include <fstream>

QGLFormat OGLplusExampleGLWidget::getGLFormat(void)
{
	QGLFormat result;
	result.setVersion(3, 3);
	return result;
}

OGLplusExampleGLWidget::OGLplusExampleGLWidget(
	QWidget *parent_widget,
	int argn,
	char **args,
	const char* ss_path
)
 : QGLWidget(getGLFormat(), parent_widget)
 , argc(argn)
 , argv(args)
 , screenshot_path(ss_path)
{
	setMouseTracking(true);
}

OGLplusExampleGLWidget::~OGLplusExampleGLWidget(void)
{
}

QSize OGLplusExampleGLWidget::minimumSizeHint(void) const
{
	return QSize(128, 128);
}

QSize OGLplusExampleGLWidget::sizeHint(void) const
{
	return QSize(800, 600);
}

void OGLplusExampleGLWidget::setWidth(int new_width)
{
	resize(new_width, this->height());
}

void OGLplusExampleGLWidget::setHeight(int new_height)
{
	resize(this->width(), new_height);
}

void OGLplusExampleGLWidget::initializeGL(void)
{
	emit updatedGL();

	oglplus::ExampleParams params(argc, argv);
	example = oglplus::makeExample(params);
	qt_clock.restart();

	if(screenshot_path)
	{
		setFixedSize(sizeHint());
		clock.Update(example->HeatUpTime());
	}
}

void OGLplusExampleGLWidget::paintGL(void)
{
	assert(example);
	if(screenshot_path)
	{
		clock.Advance(example->FrameTime());

		example->Render(clock);
		if(clock.Time() >= example->ScreenshotTime())
		{
			glFinish();
			std::vector<char> pixels(width() * height() * 3);
			glReadPixels(
				0, 0,
				width(),
				height(),
				GL_RGB,
				GL_UNSIGNED_BYTE,
				pixels.data()
			);
			std::ofstream output(screenshot_path);
			output.write(pixels.data(), pixels.size());
			emit exampleFinished();
		}
	}
	else
	{
		// TODO: this gets fscked by system clock changes
		// going to/from DST for example
		clock.Update(double(qt_clock.elapsed())/1000.0);

		if(!example->Continue(clock))
			emit exampleFinished();
		else example->Render(clock);
		emit timeUpdated(
			clock.Now().Seconds(),
			clock.RealTime().Seconds()
		);
	}
	update();
}

void OGLplusExampleGLWidget::resizeGL(int new_width, int new_height)
{
	if(new_width < 0) new_width = 0;
	if(new_height< 0) new_height= 0;

	assert(example);
	example->Reshape(new_width, new_height);

	emit updatedGL();
}

void OGLplusExampleGLWidget::mousePressEvent(QMouseEvent *mouse_event)
{
	OGLPLUS_FAKE_USE(mouse_event);
}

void OGLplusExampleGLWidget::mouseMoveEvent(QMouseEvent *mouse_event)
{
	assert(mouse_event);
	assert(example);
	example->MouseMove(
		mouse_event->x(),
		height()-
		mouse_event->y(),
		width(),
		height()
	);
	emit mouseMoved(
		mouse_event->x(),
		height()-
		mouse_event->y()
	);
}


OGLplusExampleWindow::OGLplusExampleWindow(
	int argc,
	char **argv,
	const char* screenshot_path
)
{
	setWindowTitle(tr("OGLplus example"));

	QVBoxLayout* top_layout = new QVBoxLayout();
	top_layout->setContentsMargins(QMargins(0,0,0,0));

	QHBoxLayout *main_layout = new QHBoxLayout();

	gl_widget = new OGLplusExampleGLWidget(
		this,
		argc,
		argv,
		screenshot_path
	);
	connect(
		gl_widget, SIGNAL(updatedGL()),
		this, SLOT(updateGLInfo())
	);
	connect(
		gl_widget, SIGNAL(mouseMoved(int, int)),
		this, SLOT(updateMouseInfo(int, int))
	);
	connect(
		gl_widget, SIGNAL(timeUpdated(double, double)),
		this, SLOT(updateTimeInfo(double, double))
	);
	connect(
		gl_widget, SIGNAL(exampleFinished()),
		this, SLOT(close())
	);
	main_layout->addWidget(gl_widget, 1);


	QFormLayout *form_layout = new QFormLayout();

	info_gl_vendor = new QLabel(this);
	info_gl_vendor->setAlignment(Qt::AlignRight);
	form_layout->addRow(tr("GL Vendor"), info_gl_vendor);

	info_gl_renderer = new QLabel(this);
	info_gl_renderer->setAlignment(Qt::AlignRight);
	form_layout->addRow(tr("GL Renderer"), info_gl_renderer);

	info_gl_version = new QLabel(this);
	info_gl_version->setAlignment(Qt::AlignRight);
	form_layout->addRow(tr("GL Version"), info_gl_version);

	info_gl_width = new QLabel(this);
	info_gl_width->setAlignment(Qt::AlignRight);
	form_layout->addRow(tr("Viewport width"), info_gl_width);

	info_gl_height = new QLabel(this);
	info_gl_height->setAlignment(Qt::AlignRight);
	form_layout->addRow(tr("Viewport height"), info_gl_height);

	info_mouse_x = new QLabel(this);
	info_mouse_x->setAlignment(Qt::AlignRight);
	form_layout->addRow(tr("Mouse X"), info_mouse_x);

	info_mouse_y = new QLabel(this);
	info_mouse_y->setAlignment(Qt::AlignRight);
	form_layout->addRow(tr("Mouse Y"), info_mouse_y);

	info_sim_time = new QLabel(this);
	info_sim_time->setAlignment(Qt::AlignRight);
	form_layout->addRow(tr("Sim. time [s]"), info_sim_time);

	info_real_time = new QLabel(this);
	info_real_time->setAlignment(Qt::AlignRight);
	form_layout->addRow(tr("Real time [s]"), info_real_time);

	main_layout->addLayout(form_layout);
	main_layout->addSpacing(4);

	top_layout->addLayout(main_layout, 1);
	status_bar = new QStatusBar(this);
	status_bar->setSizeGripEnabled(true);
	top_layout->addWidget(status_bar);

	setLayout(top_layout);

	resize(sizeHint());
	gl_widget->updateGeometry();
}

void OGLplusExampleWindow::updateGLInfo(void)
{
	assert(info_gl_vendor);
	info_gl_vendor->setText((const char*)glGetString(GL_VENDOR));
	assert(info_gl_renderer);
	info_gl_renderer->setText((const char*)glGetString(GL_RENDERER));
	assert(info_gl_version);
	info_gl_version->setText((const char*)glGetString(GL_VERSION));

	assert(info_gl_width);
	info_gl_width->setText(QString::number(gl_widget->width()));
	assert(info_gl_height);
	info_gl_height->setText(QString::number(gl_widget->height()));
}

void OGLplusExampleWindow::updateMouseInfo(int mouse_x, int mouse_y)
{
	assert(info_mouse_x);
	info_mouse_x->setText(QString::number(mouse_x));
	assert(info_mouse_y);
	info_mouse_y->setText(QString::number(mouse_y));
}

void OGLplusExampleWindow::updateTimeInfo(double sim_time, double real_time)
{
	assert(info_sim_time);
	info_sim_time->setText(QString::number(sim_time));
	assert(info_real_time);
	info_real_time->setText(QString::number(real_time));
}

void OGLplusExampleWindow::keyPressEvent(QKeyEvent *e)
{
	if (e->key() == Qt::Key_Escape)
		close();
	else QWidget::keyPressEvent(e);
}

int qtgl_example_main (int argc, char ** argv)
{
	const char* screenshot_path = 0;
	if((argc >= 3) && (std::strcmp(argv[1], "--screenshot") == 0))
	{
		screenshot_path = argv[2];
		for(int a=3; a<argc; ++a)
			argv[a-2] = argv[a];
		argc -= 2;
	}

	QApplication app(argc, argv);
	OGLplusExampleWindow example_window(
		argc,
		argv,
		screenshot_path
	);
	example_window.show();
	return app.exec();
}

int main (int argc, char ** argv)
{
	return oglplus::example_main(qtgl_example_main, argc, argv);
}


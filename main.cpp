#include "main.h"

void get_line_segments(const vector_3 sphere_location,
const double sphere_radius,
const double dimension)
{
	srand(0);

	const double disk_like = 3 - dimension;

	if (1)//true == redo_line_segments)
	{
		threeD_line_segments.clear();

		for (size_t i = 0; i < n; i++)
		{
			line_segment_3 ls;
			ls.start = threeD_oscillators[i];

			for (size_t j = 0; j < n; j++)
			{
				if (i == j)
					continue;

				ls.end = threeD_oscillators[j];

				line_segment_3 ls_;

				ls_.start = ls.start;
				ls_.end = ls.start + (ls.start - ls.end).normalize() * 1e30;// end;// +(ls.end - ls.start).normalize() * 10.0f;

				threeD_line_segments.push_back(ls_);
			}
		}
	}

	threeD_line_segments_intersected.clear();

	for (size_t i = 0; i < threeD_line_segments.size(); i++)
	{
		const vector_3 dir = (threeD_line_segments[i].end - threeD_line_segments[i].start).normalize();

		if (dir.dot(sphere_location) > 0)
		{
			double mu1 = 0, mu2 = 0;

			if (RaySphere(threeD_line_segments[i].start, threeD_line_segments[i].end, sphere_location, sphere_radius, &mu1, &mu2))
			{ 
				line_segment_3 ls_;
				ls_.start = threeD_line_segments[i].start;
				ls_.end = threeD_line_segments[i].start + threeD_line_segments[i].end*mu1;

				threeD_line_segments_intersected.push_back(ls_);
			}
		}
	}

//	cout << static_cast<float>(threeD_line_segments_intersected.size()) / static_cast<float>(threeD_line_segments.size()) << ", " << endl;

	vector<vector_3> vectors;

	for (size_t i = 0; i < threeD_line_segments_intersected.size(); i++)
	{
		vector_3 v = (threeD_line_segments_intersected[i].end - threeD_line_segments_intersected[i].start);
		v.normalize();
		vectors.push_back(v);
	}

	double parallelity = 0;
	size_t count = 0;

	for (size_t i = 0; i < vectors.size() - 1; i++)
	{
		for (size_t j = (i + 1); j < vectors.size(); j++)
		{
			const double d = vectors[i].dot(vectors[j]);

			parallelity += d;
			count++;
		}
	}

//	cout << count << " " << vectors.size() * (vectors.size() - 1) / 2.0  << endl;

	parallelity /= count;

	parallelity = abs(parallelity);



	double avg_strength = pow(c_meters, disk_like);

	double a =  1.0 / pow(sphere_location.x, 2.0);
	// static_cast<double>(threeD_line_segments_intersected.size()) / static_cast<double>(threeD_line_segments.size()) 
	//
	double g = (1.0 - parallelity);// / pow(sphere_location.x, 2.0);

	//cout << g << endl;
	//cout << a << endl;
	//cout << g / a << endl;
	//cout << endl;
}






vector_3 RandomUnitVector(void)
{
	double z = static_cast<double>((rand() % 256) + 1) / 256.0f * 2.0f - 1.0f;
	double a = static_cast<double>((rand() % 256) + 1) / 256.0f * 2 * pi;
	double r = sqrt(1.0f - z * z);
	double x = r * cos(a);
	double y = r * sin(a);
	return vector_3(x, y, z).normalize();
}

vector_3 slerp(vector_3 s0, vector_3 s1, const double t)
{
	vector_3 s0_norm = s0;
	s0_norm.normalize();

	vector_3 s1_norm = s1;
	s1_norm.normalize();

	const double cos_angle = s0_norm.dot(s1_norm);
	const double angle = acos(cos_angle);

	const double p0_factor = sin((1 - t)*angle) / sin(angle);
	const double p1_factor = sin(t * angle) / sin(angle);

	return s0 * p0_factor + s1 * p1_factor;
}


int main(int argc, char **argv)
{
	//cout << setprecision(20) << endl;
	srand(0);

	if (dimension < 2)
		dimension = 2;
	else if (dimension > 3)
		dimension = 3;



	for (size_t i = 0; i < n; i++)
	{
		vector_3 rv;

		rv = RandomUnitVector();

		rv.normalize();
		rv *= emitter_radius;

		if (rand() % 2)
			rv.x = -rv.x;

		if (rand() % 2)
			rv.y = -rv.y;

		if (rand() % 2)
			rv.z = -rv.z;

		threeD_oscillators.push_back(rv);
	}

	//for (int g = 0; g < 100; g++)
	//{
	//	for (size_t i = 0; i < n; i++)
	//	{
	//		for (size_t j = 0; j < n; j++)
	//		{
	//			if (i == j)
	//				continue;

	//			vector_3 accel(0, 0, 0);

	//			vector_3 grav_dir = threeD_oscillators[i] - threeD_oscillators[j];
	//			const float d = static_cast<float>(grav_dir.length());

	//			accel = grav_dir / (d * d);

	//			threeD_oscillators[i] += accel * 0.0001f;
	//		}

	//		threeD_oscillators[i].normalize();
	//		threeD_oscillators[i] *= emitter_radius;
	//	}
	//}

	// move along arc to disk formation
	for (size_t i = 0; i < threeD_oscillators.size(); i++)
	{
		const double r = threeD_oscillators[i].length();

		vector_3 ring;

		ring.x = threeD_oscillators[i].x;
		ring.y = 0;
		ring.z = threeD_oscillators[i].z;

		ring.normalize();
		ring *= r;

		const double disk_like = 3 - dimension; // where dimension is between 3.0 and 2.0

		vector_3 s = slerp(threeD_oscillators[i], ring, disk_like);

		threeD_oscillators[i] = s;
	}






 	get_line_segments(vector_3(receiver_pos, 0, 0), receiver_radius, dimension);

	//for(double dist = receiver_pos; dist <= receiver_pos*1000.0; dist += receiver_pos)
	//	get_line_segments(vector_3(dist, 0, 0), receiver_radius, dimension);


	glutInit(&argc, argv);
	init_opengl(win_x, win_y);
	glutReshapeFunc(reshape_func);
	glutIdleFunc(idle_func);
	glutDisplayFunc(display_func);
	glutKeyboardFunc(keyboard_func);
	glutMouseFunc(mouse_func);
	glutMotionFunc(motion_func);
	glutPassiveMotionFunc(passive_motion_func);
	//glutIgnoreKeyRepeat(1);
	glutMainLoop();
	glutDestroyWindow(win_id);

	return 0;
}


void idle_func(void)
{
    glutPostRedisplay();
}

void init_opengl(const int &width, const int &height)
{
	win_x = width;
	win_y = height;

	if(win_x < 1)
		win_x = 1;

	if(win_y < 1)
		win_y = 1;

	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(win_x, win_y);
	win_id = glutCreateWindow("orbit");

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glClearColor(static_cast<float>(background_colour.x), static_cast<float>(background_colour.y), static_cast<float>(background_colour.z), 1);
	glClearDepth(1.0f);

	main_camera.Set(0, 0, camera_w, camera_fov, win_x, win_y, camera_near, camera_far);
}

void reshape_func(int width, int height)
{
	win_x = width;
	win_y = height;

	if(win_x < 1)
		win_x = 1;

	if(win_y < 1)
		win_y = 1;

	glutSetWindow(win_id);
	glutReshapeWindow(win_x, win_y);
	glViewport(0, 0, win_x, win_y);

	main_camera.Set(main_camera.u, main_camera.v, main_camera.w, main_camera.fov, win_x, win_y, camera_near, camera_far);
}

// Text drawing code originally from "GLUT Tutorial -- Bitmap Fonts and Orthogonal Projections" by A R Fernandes
void render_string(int x, const int y, void *font, const string &text)
{
	for(size_t i = 0; i < text.length(); i++)
	{
		glRasterPos2i(x, y);
		glutBitmapCharacter(font, text[i]);
		x += glutBitmapWidth(font, text[i]) + 1;
	}
}
// End text drawing code.

void draw_objects(void)
{
    glDisable(GL_LIGHTING);
    
	glPushMatrix();
  
	glScaled(1.0 / receiver_radius, 1.0 / receiver_radius, 1.0 / receiver_radius);
	glPointSize(1.0);
	glLineWidth(1.0f);
	

	glBegin(GL_POINTS);

	glColor3f(1, 1, 1);

	for (size_t i = 0; i < threeD_oscillators.size(); i++)
		glVertex3d(threeD_oscillators[i].x, threeD_oscillators[i].y, threeD_oscillators[i].z);

	glEnd();



	glEnable(GL_ALPHA);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//glBegin(GL_LINES);

	//glColor4f(0, 1, 0, 0.1f);

	//for (size_t i = 0; i < threeD_line_segments.size(); i++)
	//{
	//	if(threeD_line_segments[i].start.z > 0 || threeD_line_segments[i].end.z > 0)
	//		continue;

	//	glVertex3d(threeD_line_segments[i].start.x, threeD_line_segments[i].start.y, threeD_line_segments[i].start.z);
	//	glVertex3d(threeD_line_segments[i].end.x, threeD_line_segments[i].end.y, threeD_line_segments[i].end.z);
	//}

	//glEnd();

	glBegin(GL_LINES);

	glColor4f(0, 0, 1, 0.1f);

	for (size_t i = 0; i < threeD_line_segments_intersected.size(); i++)
	{
		glVertex3d(threeD_line_segments_intersected[i].start.x, threeD_line_segments_intersected[i].start.y, threeD_line_segments_intersected[i].start.z);
		glVertex3d(threeD_line_segments_intersected[i].end.x, threeD_line_segments_intersected[i].end.y, threeD_line_segments_intersected[i].end.z);
	}

	glEnd();

	glDisable(GL_BLEND);



	// If we do draw the axis at all, make sure not to draw its outline.
	if(true == draw_axis)
	{
		glBegin(GL_LINES);

		glColor3f(1, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(static_cast<float>(emitter_radius), 0, 0);
		glColor3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, static_cast<float>(emitter_radius), 0);
		glColor3f(0, 0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, static_cast<float>(emitter_radius));

		glEnd();
	}

	glPopMatrix();
}




void display_func(void)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	// Draw the model's components using OpenGL/GLUT primitives.
	draw_objects();

	if(true == draw_control_list)
	{
		// Text drawing code originally from "GLUT Tutorial -- Bitmap Fonts and Orthogonal Projections" by A R Fernandes
		// http://www.lighthouse3d.com/opengl/glut/index.php?bmpfontortho
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, static_cast<float>(win_x), 0, static_cast<float>(win_y));
		glScalef(1, -1, 1); // Neat. :)
		glTranslatef(0, -static_cast<float>(win_y), 0); // Neat. :)
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		glColor3d(control_list_colour.x, control_list_colour.y, control_list_colour.z);

		int break_size = 22;
		int start = 20;
		ostringstream oss;

		render_string(10, start, GLUT_BITMAP_HELVETICA_18, string("Mouse controls:"));
		render_string(10, start + 1*break_size, GLUT_BITMAP_HELVETICA_18, string("  LMB + drag: Rotate camera"));
		render_string(10, start + 2*break_size, GLUT_BITMAP_HELVETICA_18, string("  RMB + drag: Zoom camera"));

		render_string(10, start + 4*break_size, GLUT_BITMAP_HELVETICA_18, string("Keyboard controls:"));
        render_string(10, start + 5*break_size, GLUT_BITMAP_HELVETICA_18, string("  w: Draw axis"));
		render_string(10, start + 6*break_size, GLUT_BITMAP_HELVETICA_18, string("  e: Draw text"));
		render_string(10, start + 7*break_size, GLUT_BITMAP_HELVETICA_18, string("  u: Rotate camera +u"));
		render_string(10, start + 8*break_size, GLUT_BITMAP_HELVETICA_18, string("  i: Rotate camera -u"));
		render_string(10, start + 9*break_size, GLUT_BITMAP_HELVETICA_18, string("  o: Rotate camera +v"));
		render_string(10, start + 10*break_size, GLUT_BITMAP_HELVETICA_18, string("  p: Rotate camera -v"));


		
        custom_math::vector_3 eye = main_camera.eye;
		custom_math::vector_3 eye_norm = eye;
		eye_norm.normalize();

		oss.clear();
		oss.str("");		
		oss << "Camera position: " << eye.x << ' ' << eye.y << ' ' << eye.z;
		render_string(10, win_y - 2*break_size, GLUT_BITMAP_HELVETICA_18, oss.str());

		oss.clear();
		oss.str("");
		oss << "Camera position (normalized): " << eye_norm.x << ' ' << eye_norm.y << ' ' << eye_norm.z;
		render_string(10, win_y - break_size, GLUT_BITMAP_HELVETICA_18, oss.str());

		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		// End text drawing code.
	}

	glFlush();
	glutSwapBuffers();
}

void keyboard_func(unsigned char key, int x, int y)
{
	switch(tolower(key))
	{
	case 'w':
		{
			draw_axis = !draw_axis;
			break;
		}
	case 'e':
		{
			draw_control_list = !draw_control_list;
			break;
		}
	case 'u':
		{
			main_camera.u -= u_spacer;
			main_camera.Set();
			break;
		}
	case 'i':
		{
			main_camera.u += u_spacer;
			main_camera.Set();
			break;
		}
	case 'o':
		{
			main_camera.v -= v_spacer;
			main_camera.Set();
			break;
		}
	case 'p':
		{
			main_camera.v += v_spacer;
			main_camera.Set();
			break;
		}

	case ' ':
		{
		//repulse();

			break;
		}




	default:
		break;
	}
}

void mouse_func(int button, int state, int x, int y)
{
	if(GLUT_LEFT_BUTTON == button)
	{
		if(GLUT_DOWN == state)
			lmb_down = true;
		else
			lmb_down = false;
	}
	else if(GLUT_MIDDLE_BUTTON == button)
	{
		if(GLUT_DOWN == state)
			mmb_down = true;
		else
			mmb_down = false;
	}
	else if(GLUT_RIGHT_BUTTON == button)
	{
		if(GLUT_DOWN == state)
			rmb_down = true;
		else
			rmb_down = false;
	}
}

void motion_func(int x, int y)
{
	int prev_mouse_x = mouse_x;
	int prev_mouse_y = mouse_y;

	mouse_x = x;
	mouse_y = y;

	int mouse_delta_x = mouse_x - prev_mouse_x;
	int mouse_delta_y = prev_mouse_y - mouse_y;

	if(true == lmb_down && (0 != mouse_delta_x || 0 != mouse_delta_y))
	{
		main_camera.u -= static_cast<float>(mouse_delta_y)*u_spacer;
		main_camera.v += static_cast<float>(mouse_delta_x)*v_spacer;
	}
	else if(true == rmb_down && (0 != mouse_delta_y))
	{
		main_camera.w -= static_cast<float>(mouse_delta_y)*w_spacer;

		if(main_camera.w < 2.0f)
			main_camera.w = 2.0f;

	}

	main_camera.Set(); // Calculate new camera vectors.
}

void passive_motion_func(int x, int y)
{
	mouse_x = x;
	mouse_y = y;
}





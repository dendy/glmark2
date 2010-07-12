#include "mesh.h"
#include "shader.h"

Texel::Texel()
{
    u = 0;  v = 0;
}

Texel::Texel(GLfloat pU, GLfloat pV)
{
    u = pU; v = pV;
}
    
Mesh::Mesh()
{
    mPolygonQty = 0;
    mVertexQty = 0;
    mMode = GL_TRIANGLES;
    mVertex = 0;
}

Mesh::~Mesh()
{
    delete [] mVertex;
    //deleteArray
}

void Mesh::make_cube()
{
    fprintf(stderr, "Warning: %s: Not implemented\n", __FUNCTION__);
}

void Mesh::make_torus()
{
    unsigned wraps_qty = 64;
    unsigned per_wrap_qty = 64;
    float major_radius = 0.8;
    float minor_radius = 0.4;
    unsigned i, j;
    unsigned k = 0;
    
    Vector3f a, b, c, d, n;

    mMode = GL_TRIANGLES;
    mVertexQty = wraps_qty * per_wrap_qty * 6;
    mVertex = new Vertex[mVertexQty];
    
    for(i = 0; i < wraps_qty; i++)
        for(j = 0; j < per_wrap_qty; j++)
        {
            float wrap_frac = j / (float)per_wrap_qty;
            float phi = 2 * M_PI * wrap_frac;
            float theta = 2 * M_PI * (i + wrap_frac) / (float)wraps_qty;
            float r = major_radius + minor_radius * (float)cos(phi);
            a.x = (float)sin(theta) * r;
            a.y = minor_radius * (float)sin(phi);
            a.z = (float)cos(theta) * r;
            
            theta = 2 * M_PI * (i + wrap_frac + 1) / (float)wraps_qty;
            b.x = (float)sin(theta) * r;
            b.y = minor_radius * (float)sin(phi);
            b.z = (float)cos(theta) * r;

            wrap_frac = (j + 1) / (float)per_wrap_qty;
            phi = 2 * M_PI * wrap_frac;
            theta = 2 * M_PI * (i + wrap_frac) / (float)wraps_qty;
            r = major_radius + minor_radius * (float)cos(phi);
            c.x = (float)sin(theta) * r;
            c.y = minor_radius * (float)sin(phi);
            c.z = (float)cos(theta) * r;

            theta = 2 * M_PI * (i + wrap_frac + 1) / (float)wraps_qty;
            d.x = (float)sin(theta) * r;
            d.y = minor_radius * (float)sin(phi);
            d.z = (float)cos(theta) * r;

            n = normal(a, b, c);
            mVertex[k].n = n;   mVertex[k].v = a;   k++;
            mVertex[k].n = n;   mVertex[k].v = b;   k++;
            mVertex[k].n = n;   mVertex[k].v = c;   k++;
            n = normal(a, b, c);
            mVertex[k].n = n;   mVertex[k].v = b;   k++;
            mVertex[k].n = n;   mVertex[k].v = c;   k++;
            mVertex[k].n = n;   mVertex[k].v = d;   k++;            
        }    
}

void Mesh::render_array()
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT, sizeof(Vertex), &mVertex[0].v.x);
    glNormalPointer(GL_FLOAT, sizeof(Vertex), &mVertex[0].n.x);
    glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &mVertex[0].t.u);
    glDrawArrays(mMode, 0, mVertexQty);

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

void Mesh::render_array_attrib()
{
    // Enable the attributes
    glEnableVertexAttribArray(Shader::VertexAttribLocation);
    glEnableVertexAttribArray(Shader::NormalAttribLocation);
    glEnableVertexAttribArray(Shader::TexCoordAttribLocation);

    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
    glVertexAttribPointer(Shader::VertexAttribLocation, 3, GL_FLOAT,
                          GL_FALSE, sizeof(Vertex), &mVertex[0].v.x);
    glVertexAttribPointer(Shader::NormalAttribLocation, 3, GL_FLOAT,
                          GL_FALSE, sizeof(Vertex), &mVertex[0].n.x);
    glVertexAttribPointer(Shader::TexCoordAttribLocation, 2, GL_FLOAT,
                          GL_FALSE, sizeof(Vertex), &mVertex[0].t.u);

    glDrawArrays(GL_TRIANGLES, 0, mVertexQty);

    // Disable the attributes
    glDisableVertexAttribArray(Shader::TexCoordAttribLocation);
    glDisableVertexAttribArray(Shader::NormalAttribLocation);
    glDisableVertexAttribArray(Shader::VertexAttribLocation);
}

void Mesh::build_vbo()
{
#ifdef _DEBUG
    printf("Building vbo for mesh...         ");
#endif

    Vector3f *vertex;
    Texel *texel;
    Vector3f *normal;

    vertex = new Vector3f[mVertexQty];
    texel = new Texel[mVertexQty];
    normal = new Vector3f[mVertexQty];

    for(unsigned i = 0; i < mVertexQty; i++)
    {
        vertex[i] = mVertex[i].v;
        texel[i] = mVertex[i].t;
        normal[i] = mVertex[i].n;
    }

    // Generate And Bind The Vertex Buffer
    glGenBuffersARB(1, &mVBOVertices);                          // Get A Valid Name
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, mVBOVertices);         // Bind The Buffer
    // Load The Data
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, mVertexQty * sizeof(Vector3f), vertex, GL_STATIC_DRAW_ARB);

    // Generate And Bind The normal Buffer
    glGenBuffersARB(1, &mVBONormals);                          // Get A Valid Name
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, mVBONormals);         // Bind The Buffer
    // Load The Data
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, mVertexQty * sizeof(Vector3f), normal, GL_STATIC_DRAW_ARB);

    // Generate And Bind The Texture Coordinate Buffer
    glGenBuffersARB(1, &mVBOTexCoords);                       // Get A Valid Name
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, mVBOTexCoords);      // Bind The Buffer
    // Load The Data
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, mVertexQty * sizeof(Texel), texel, GL_STATIC_DRAW_ARB);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    delete [] vertex;
    delete [] texel;
    delete [] normal;
#ifdef _DEBUG
    printf("[ Done ]\n");
#endif
}

void Mesh::render_vbo()
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glBindBufferARB(GL_ARRAY_BUFFER_ARB, mVBOVertices);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, mVBONormals);
    glNormalPointer(GL_FLOAT, 0, 0);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, mVBOTexCoords);
    glTexCoordPointer(2, GL_FLOAT, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, mVertexQty);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void Mesh::render_vbo_attrib()
{
    // Enable the attributes
    glEnableVertexAttribArray(Shader::VertexAttribLocation);
    glEnableVertexAttribArray(Shader::NormalAttribLocation);
    glEnableVertexAttribArray(Shader::TexCoordAttribLocation);

    glBindBufferARB(GL_ARRAY_BUFFER_ARB, mVBOVertices);
    glVertexAttribPointer(Shader::VertexAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, mVBONormals);
    glVertexAttribPointer(Shader::NormalAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, mVBOTexCoords);
    glVertexAttribPointer(Shader::TexCoordAttribLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, mVertexQty);

    // Disable the attributes
    glDisableVertexAttribArray(Shader::TexCoordAttribLocation);
    glDisableVertexAttribArray(Shader::NormalAttribLocation);
    glDisableVertexAttribArray(Shader::VertexAttribLocation);
}

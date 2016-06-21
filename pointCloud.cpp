#include <omega.h>
#include <cyclops/cyclops.h>
#include <omegaToolkit.h>

#include "TextPointsLoader.h"
#include "BinaryPointsLoader.h"
#include "pointCloud.h"

using namespace omega;
using namespace cyclops;
using namespace omegaToolkit;
///////////////////////////////////////////////////////////////////////////////
// Python wrapper code.
#ifdef OMEGA_USE_PYTHON
#include "omega/PythonInterpreterWrapper.h"
BOOST_PYTHON_MODULE(pointCloud)
{
    PYAPI_REF_CLASS_WITH_CTOR(TextPointsLoader, ModelLoader);
    PYAPI_REF_CLASS_WITH_CTOR(BinaryPointsLoader, ModelLoader);


    PYAPI_REF_BASE_CLASS(PointCloud)
        PYAPI_STATIC_REF_GETTER(PointCloud, createAndInitialize)
        PYAPI_REF_GETTER(PointCloud, getStaticObject)
        ;

}
#endif



///////////////////////////////////////////////////////////////////////////////////////////////////
PointCloud* PointCloud::createAndInitialize(SceneManager* sceneMng, std::string modelName, std::string modelPath, std::string  modelExtension, float pointSize )
{
    PointCloud* instance = new PointCloud(sceneMng, modelName, modelPath, modelExtension, pointSize);
    ModuleServices::addModule(instance);
    instance->doInitialize(Engine::instance());
    return instance;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PointCloud::PointCloud(SceneManager* sceneMng, std::string modelName, std::string modelPath, std::string  modelExtension, float pointSize ):
    EngineModule("PointCloud"),
    m_sceneManager(sceneMng),
    m_modelName(modelName),
    m_modelPath(modelPath),
    m_modelExtension(modelExtension),
    m_pointSize(pointSize)
{
}


///////////////////////////////////////////////////////////////////////////////////////////////////


class DrawableVisitor : public osg::NodeVisitor
{
    private:
    float m_pointSize;
public:


    DrawableVisitor(float pointSize) : m_pointSize(pointSize) 
    {
        setTraversalMode( osg::NodeVisitor::TRAVERSE_ALL_CHILDREN );
    }


    void apply( osg::Geode& geode )
    {
        for ( unsigned int i=0; i<geode.getNumDrawables(); ++i )
        {
             osg::Geometry* geom = dynamic_cast<osg::Geometry*>( geode.getDrawable(i) );
             if (!geom)
                continue;

            geom->getOrCreateStateSet()->setAttributeAndModes( new osg::Point(m_pointSize) );
            osg::PointSprite *sprite = new osg::PointSprite();
            geom->getOrCreateStateSet()->setTextureAttributeAndModes(0, sprite, osg::StateAttribute::ON);
        }
    }
};

void PointCloud::initialize()
{
    osg::Node* pLoadedModel = osgDB::readNodeFile(m_modelPath + "." + m_modelExtension); 


    if (pLoadedModel != NULL)
        std::cout << "finished loading model" << std::endl;
    else {
        std::cout << "error loading model" << std::endl;
        return;
    }

    DrawableVisitor visitor(m_pointSize);
    pLoadedModel->accept(visitor);

    m_modelAsset = new ModelAsset();
    m_modelAsset->name = m_modelName;
    m_modelAsset->numNodes = 1;
    m_modelAsset->info = NULL;
    m_modelAsset->nodes.push_back(pLoadedModel);

    m_sceneManager->addModelAsset(m_modelAsset);
    m_staticObject = new StaticObject(m_sceneManager, m_modelName);
    m_staticObject->setName(m_modelName);

}

StaticObject* PointCloud::getStaticObject(){
    return m_staticObject.get();
}

void PointCloud::update(const UpdateContext& context)
{
}





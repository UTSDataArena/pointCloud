#include <omega.h>
#include <cyclops/cyclops.h>


#include <osg/Point>
#include <osg/Group>
#include <osgDB/ReadFile>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/Viewer>
#include <fstream>
#include <iostream>



///////////////////////////////////////////////////////////////////////////////////////////////////
// Module declaration
class PointCloud: public omega::EngineModule
{
public:
	static PointCloud* createAndInitialize(cyclops::SceneManager* sceneMng, std::string modelName, std::string modelPath, std::string  modelExtension, float pointSize);
	PointCloud(cyclops::SceneManager* sceneMng, std::string modelName, std::string modelPath, std::string  modelExtension, float pointSize);

	virtual void initialize();
	virtual void update(const cyclops::UpdateContext& context);

	cyclops::StaticObject* getStaticObject();

protected:
	cyclops::Ref<cyclops::SceneManager> m_sceneManager;
	// Ref<StaticObject> m_pointCloudObject;

	cyclops::Ref<cyclops::ModelAsset> m_modelAsset;
	cyclops::Ref<cyclops::StaticObject> m_staticObject;
	std::string m_modelName;
	std::string m_modelPath;
	std::string  m_modelExtension;
	float m_pointSize;
};



class InstancedPointCloud: public omega::EngineModule
{
public:
	static InstancedPointCloud* createAndInitialize(cyclops::SceneManager* sceneMng, std::string modelName, std::string modelPath, std::string  modelExtension, float pointSize);
	InstancedPointCloud(cyclops::SceneManager* sceneMng, std::string modelName, std::string modelPath, std::string  modelExtension, float pointSize);

	void createInstancedGeometry( osg::Image* img,  osg::Image* colorImg, unsigned int numInstances );
	bool readPointDataTxt( const std::string& file, std::vector<float>& imageData, osg::BoundingBox& boundBox) ;
	bool readPointModel(const std::string& file, std::vector<float>& imageData, std::vector<unsigned char>& vertColorData, osg::BoundingBox& boundBox);
	bool readPointData( const std::string& file, const std::string& extension, float pointSize);

	virtual void initialize();
		virtual void update(const cyclops::UpdateContext& context);
	// virtual void update(const cyclops::UpdateContext& context);

	cyclops::ModelGeometry* getModelGeometry();

protected:
	cyclops::Ref<cyclops::SceneManager> m_sceneManager;
	// Ref<StaticObject> m_pointCloudObject;

	cyclops::Ref<cyclops::ModelGeometry> m_modelGeom;
	std::string m_modelName;
	std::string m_modelPath;
	std::string  m_modelExtension;
	float m_pointSize;
};




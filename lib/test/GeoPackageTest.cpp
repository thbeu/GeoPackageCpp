#include <sstream>
#include <iostream>
#include <filesystem>
#include "gtest/gtest.h"
#include "GeoPackage.hpp"

// GeoPackage

TEST(GeoPackageLibTests, GeoPackage_Create) {
    const std::string fileName = "tiles.gpkg";
    geopackage::GeoPackage gpkg { fileName };
    EXPECT_TRUE(std::filesystem::exists(fileName));
    EXPECT_TRUE(std::filesystem::remove(fileName));
}

// Size

TEST(GeoPackageLibTests, Size_Int_Create) {
    geopackage::Size<int> intSize {10,12};
    EXPECT_EQ(10, intSize.getWidth());
    EXPECT_EQ(12, intSize.getHeight());
    std::stringstream str;
    str << intSize;
    EXPECT_EQ("SIZE (10, 12)", str.str());
}

TEST(GeoPackageLibTests, Size_Double_Create) {
    geopackage::Size<double> size {1.2,3.4};
    EXPECT_EQ(1.2, size.getWidth());
    EXPECT_EQ(3.4, size.getHeight());
    std::stringstream str;
    str << size;
    EXPECT_EQ("SIZE (1.2, 3.4)", str.str());
}

// SpatialRef

TEST(GeoPackageLibTests, SpatialRef_ToString) {
    geopackage::SpatialRef s{"WGS84", 4326, "EPSG", 4326, "GEOGCS[]", "longitude/latitude coordinates in decimal degrees on the WGS 84 spheroid"};
    std::stringstream str;
    str << s;
    EXPECT_EQ("SPATIAL_REF (srsName = WGS84, srsId = 4326)", str.str());
}

TEST(GeoPackageLibTests, GeoPackage_Add_SpatialRef) {
    const std::string fileName = "data.gpkg";
    geopackage::GeoPackage gpkg { fileName };
    EXPECT_TRUE(std::filesystem::exists(fileName));

    gpkg.addSpatialRef(geopackage::SpatialRef{"WGS84", 4326, "EPSG", 4326, "GEOGCS[]", "longitude/latitude coordinates in decimal degrees on the WGS 84 spheroid"});
    std::optional<geopackage::SpatialRef> m = gpkg.getSpatialRef(4326);
    EXPECT_TRUE(m.has_value());

    EXPECT_TRUE(std::filesystem::remove(fileName));
}

TEST(GeoPackageLibTests, GeoPackage_Update_SpatialRef) {
    const std::string fileName = "data.gpkg";
    geopackage::GeoPackage geopackage { fileName };
    EXPECT_TRUE(std::filesystem::exists(fileName));

    geopackage.addSpatialRef(geopackage::SpatialRef{"WGS84", 4326, "EPSG", 4326, "GEOGCS[]", "longitude/latitude coordinates in decimal degrees on the WGS 84 spheroid"});
    std::optional<geopackage::SpatialRef> s1 = geopackage.getSpatialRef(4326);
    EXPECT_TRUE(s1.has_value());
    EXPECT_EQ("WGS84", s1.value().getSrsName());

    geopackage.updateSpatialRef(geopackage::SpatialRef{"EPSG:4326", 4326, "EPSG", 4326, "GEOGCS[]", "longitude/latitude coordinates in decimal degrees on the WGS 84 spheroid"});
    std::optional<geopackage::SpatialRef> s2 = geopackage.getSpatialRef(4326);
    EXPECT_TRUE(s2.has_value());
    EXPECT_EQ("EPSG:4326", s2.value().getSrsName());

    EXPECT_TRUE(std::filesystem::remove(fileName));
}

TEST(GeoPackageLibTests, GeoPackage_Set_SpatialRef) {
    const std::string fileName = "data.gpkg";
    geopackage::GeoPackage geopackage { fileName };
    EXPECT_TRUE(std::filesystem::exists(fileName));

    geopackage.setSpatialRef(geopackage::SpatialRef{"WGS84", 4326, "EPSG", 4326, "GEOGCS[]", "longitude/latitude coordinates in decimal degrees on the WGS 84 spheroid"});
    std::optional<geopackage::SpatialRef> s1 = geopackage.getSpatialRef(4326);
    EXPECT_TRUE(s1.has_value());
    EXPECT_EQ("WGS84", s1.value().getSrsName());

    geopackage.setSpatialRef(geopackage::SpatialRef{"EPSG:4326", 4326, "EPSG", 4326, "GEOGCS[]", "longitude/latitude coordinates in decimal degrees on the WGS 84 spheroid"});
    std::optional<geopackage::SpatialRef> s2 = geopackage.getSpatialRef(4326);
    EXPECT_TRUE(s2.has_value());
    EXPECT_EQ("EPSG:4326", s2.value().getSrsName());

    EXPECT_TRUE(std::filesystem::remove(fileName));
}

TEST(GeoPackageLibTests, GeoPackage_Delete_SpatialRef) {
    const std::string fileName = "data.gpkg";
    geopackage::GeoPackage geopackage { fileName };
    EXPECT_TRUE(std::filesystem::exists(fileName));

    geopackage.addSpatialRef(geopackage::SpatialRef{"WGS84", 4326, "EPSG", 4326, "GEOGCS[]", "longitude/latitude coordinates in decimal degrees on the WGS 84 spheroid"});
    std::optional<geopackage::SpatialRef> s1 = geopackage.getSpatialRef(4326);
    EXPECT_TRUE(s1.has_value());
    EXPECT_EQ("WGS84", s1.value().getSrsName());

    geopackage.deleteSpatialRef(s1.value());
    std::optional<geopackage::SpatialRef> s2 = geopackage.getSpatialRef(4326);
    EXPECT_FALSE(s2.has_value());

    EXPECT_TRUE(std::filesystem::remove(fileName));
}

TEST(GeoPackageLibTests, GeoPackage_Get_SpatialRef) {
    const std::string fileName = "data.gpkg";
    geopackage::GeoPackage geopackage { fileName };
    EXPECT_TRUE(std::filesystem::exists(fileName));

    std::optional<geopackage::SpatialRef> s1 = geopackage.getSpatialRef(4326);
    EXPECT_FALSE(s1.has_value());
    geopackage.addSpatialRef(geopackage::SpatialRef{"WGS84", 4326, "EPSG", 4326, "GEOGCS[]", "longitude/latitude coordinates in decimal degrees on the WGS 84 spheroid"});
    std::optional<geopackage::SpatialRef> s2 = geopackage.getSpatialRef(4326);
    EXPECT_TRUE(s2.has_value());

    EXPECT_TRUE(std::filesystem::remove(fileName));
}

TEST(GeoPackageLibTests, GeoPackage_SpatialRefs) {
    const std::string fileName = "data.gpkg";
    geopackage::GeoPackage geopackage { fileName };
    EXPECT_TRUE(std::filesystem::exists(fileName));

    geopackage.addSpatialRef(geopackage::SpatialRef{"WGS84", 4326, "EPSG", 4326, "GEOGCS[]", "longitude/latitude coordinates in decimal degrees on the WGS 84 spheroid"});
    geopackage.addSpatialRef(geopackage::SpatialRef{"EPSG:2927", 2927, "EPSG", 2927, "PROJ[]", "wa state plane south"});
    
    int counter = 0;    
    geopackage.spatialRefs([&](geopackage::SpatialRef& s) {
        counter++;
    });
    EXPECT_EQ(2, counter);


    EXPECT_TRUE(std::filesystem::remove(fileName));
}

// TileMatrixSet

TEST(GeoPackageLibTests, TileMatrixSet_ToString) {
    geopackage::TileMatrixSet tileMatrixSet{"basemap", 4326, geopackage::Bounds{-180,-90,180,90}};
    std::stringstream str;
    str << tileMatrixSet;
    EXPECT_EQ("TILE_MATRIX_SET (tableName = basemap, srsId = 4326, bounds = BOUNDS (-180, -180, 180, 90))", str.str());
}

TEST(GeoPackageLibTests, GeoPackage_Add_TileMatrixSet) {
    const std::string fileName = "data.gpkg";
    geopackage::GeoPackage gpkg { fileName };
    EXPECT_TRUE(std::filesystem::exists(fileName));

    gpkg.addTileMatrixSet(geopackage::TileMatrixSet{"basemap", 4326, geopackage::Bounds{-180,-90,180,90}});
    std::optional<geopackage::TileMatrixSet> tileMatrixSet = gpkg.getTileMatrixSet("basemap");
    EXPECT_TRUE(tileMatrixSet.has_value());

    EXPECT_TRUE(std::filesystem::remove(fileName));
}

TEST(GeoPackageLibTests, GeoPackage_Update_TileMatrixSet) {
    const std::string fileName = "data.gpkg";
    geopackage::GeoPackage geopackage { fileName };
    EXPECT_TRUE(std::filesystem::exists(fileName));

    geopackage.addTileMatrixSet(geopackage::TileMatrixSet{"basemap", 4326, geopackage::Bounds{-180,-90,180,90}});
    std::optional<geopackage::TileMatrixSet> s1 = geopackage.getTileMatrixSet("basemap");
    EXPECT_TRUE(s1.has_value());
    EXPECT_EQ(4326, s1.value().getSrsId());

    geopackage.updateTileMatrixSet(geopackage::TileMatrixSet{"basemap", 3857, geopackage::Bounds{-180,-85,180,85}});
    std::optional<geopackage::TileMatrixSet> s2 = geopackage.getTileMatrixSet("basemap");
    EXPECT_TRUE(s2.has_value());
    EXPECT_EQ(3857, s2.value().getSrsId());

    EXPECT_TRUE(std::filesystem::remove(fileName));
}

TEST(GeoPackageLibTests, GeoPackage_Set_TileMatrixSet) {
    const std::string fileName = "data.gpkg";
    geopackage::GeoPackage geopackage { fileName };
    EXPECT_TRUE(std::filesystem::exists(fileName));

    geopackage.setTileMatrixSet(geopackage::TileMatrixSet{"basemap", 4326, geopackage::Bounds{-180,-90,180,90}});
    std::optional<geopackage::TileMatrixSet> s1 = geopackage.getTileMatrixSet("basemap");
    EXPECT_TRUE(s1.has_value());
    EXPECT_EQ(4326, s1.value().getSrsId());

    geopackage.setTileMatrixSet(geopackage::TileMatrixSet{"basemap", 3857, geopackage::Bounds{-180,-85,180,85}});
    std::optional<geopackage::TileMatrixSet> s2 = geopackage.getTileMatrixSet("basemap");
    EXPECT_TRUE(s2.has_value());
    EXPECT_EQ(3857, s2.value().getSrsId());

    EXPECT_TRUE(std::filesystem::remove(fileName));
}

TEST(GeoPackageLibTests, GeoPackage_Delete_TileMatrixSet) {
    const std::string fileName = "data.gpkg";
    geopackage::GeoPackage geopackage { fileName };
    EXPECT_TRUE(std::filesystem::exists(fileName));

    geopackage.addTileMatrixSet(geopackage::TileMatrixSet{"basemap", 4326, geopackage::Bounds{-180,-90,180,90}});
    std::optional<geopackage::TileMatrixSet> s1 = geopackage.getTileMatrixSet("basemap");
    EXPECT_TRUE(s1.has_value());
    EXPECT_EQ(4326, s1.value().getSrsId());

    geopackage.deleteTileMatrixSet(s1.value());
    std::optional<geopackage::TileMatrixSet> s2 = geopackage.getTileMatrixSet("basemap");
    EXPECT_FALSE(s2.has_value());

    EXPECT_TRUE(std::filesystem::remove(fileName));
}

TEST(GeoPackageLibTests, GeoPackage_Get_TileMatrixSet) {
    const std::string fileName = "data.gpkg";
    geopackage::GeoPackage geopackage { fileName };
    EXPECT_TRUE(std::filesystem::exists(fileName));

    std::optional<geopackage::TileMatrixSet> s1 = geopackage.getTileMatrixSet("basemap");
    EXPECT_FALSE(s1.has_value());
    geopackage.addTileMatrixSet(geopackage::TileMatrixSet{"basemap", 4326, geopackage::Bounds{-180,-90,180,90}});
    std::optional<geopackage::TileMatrixSet> s2 = geopackage.getTileMatrixSet("basemap");
    EXPECT_TRUE(s2.has_value());

    EXPECT_TRUE(std::filesystem::remove(fileName));
}

TEST(GeoPackageLibTests, GeoPackage_TileMatrixSets) {
    const std::string fileName = "data.gpkg";
    geopackage::GeoPackage geopackage { fileName };
    EXPECT_TRUE(std::filesystem::exists(fileName));

    geopackage.addTileMatrixSet(geopackage::TileMatrixSet{"basemap", 4326, geopackage::Bounds{-180,-90,180,90}});
    geopackage.addTileMatrixSet(geopackage::TileMatrixSet{"mercator", 3857, geopackage::Bounds{-180,-85,180,85}});
    
    int counter = 0;    
    geopackage.tileMatrixSets([&](geopackage::TileMatrixSet& s) {
        counter++;
    });
    EXPECT_EQ(2, counter);


    EXPECT_TRUE(std::filesystem::remove(fileName));
}

// Tile Matrix

TEST(GeoPackageLibTests, TileMatrix_ToString) {
    geopackage::TileMatrix tileMatrix{"basemap", 0, geopackage::Size<int>{1, 1}, geopackage::Size<int>{256, 256}, geopackage::Size<double>{2.0, 2.0}};
    std::stringstream str;
    str << tileMatrix;
    EXPECT_EQ("TILE_MATRIX (tableName = basemap, zoomLevel = 0, matrixSize = SIZE (1, 1), tileSize = SIZE (256, 256), pixelSize = SIZE (2, 2))", str.str());
}

TEST(GeoPackageLibTests, GeoPackage_Add_TileMatrix) {
    const std::string fileName = "data.gpkg";
    geopackage::GeoPackage gpkg { fileName };
    EXPECT_TRUE(std::filesystem::exists(fileName));

    gpkg.addTileMatrix(geopackage::TileMatrix{"basemap", 0, geopackage::Size<int>{1, 1}, geopackage::Size<int>{256, 256}, geopackage::Size<double>{2.0, 2.0}});
    std::optional<geopackage::TileMatrix> m = gpkg.getTileMatrix("basemap", 0);
    EXPECT_TRUE(m.has_value());

    EXPECT_TRUE(std::filesystem::remove(fileName));
}

TEST(GeoPackageLibTests, GeoPackage_Update_TileMatrix) {
    const std::string fileName = "data.gpkg";
    geopackage::GeoPackage geopackage { fileName };
    EXPECT_TRUE(std::filesystem::exists(fileName));

    geopackage.addTileMatrix(geopackage::TileMatrix{"basemap", 0, geopackage::Size<int>{1, 1}, geopackage::Size<int>{256, 256}, geopackage::Size<double>{2.0, 2.0}});
    std::optional<geopackage::TileMatrix> s1 = geopackage.getTileMatrix("basemap", 0);
    EXPECT_TRUE(s1.has_value());
    EXPECT_EQ(1, s1.value().getMatrixSize().getWidth());

    geopackage.updateTileMatrix(geopackage::TileMatrix{"basemap", 0, geopackage::Size<int>{2, 2}, geopackage::Size<int>{256, 256}, geopackage::Size<double>{2.0, 2.0}});
    std::optional<geopackage::TileMatrix> s2 = geopackage.getTileMatrix("basemap", 0);
    EXPECT_TRUE(s2.has_value());
    EXPECT_EQ(2, s2.value().getMatrixSize().getWidth());

    EXPECT_TRUE(std::filesystem::remove(fileName));
}

TEST(GeoPackageLibTests, GeoPackage_Set_TileMatrix) {
    const std::string fileName = "data.gpkg";
    geopackage::GeoPackage geopackage { fileName };
    EXPECT_TRUE(std::filesystem::exists(fileName));

    geopackage.setTileMatrix(geopackage::TileMatrix{"basemap", 0, geopackage::Size<int>{1, 1}, geopackage::Size<int>{256, 256}, geopackage::Size<double>{2.0, 2.0}});
    std::optional<geopackage::TileMatrix> s1 = geopackage.getTileMatrix("basemap", 0);
    EXPECT_TRUE(s1.has_value());
    EXPECT_EQ(1, s1.value().getMatrixSize().getWidth());

    geopackage.setTileMatrix(geopackage::TileMatrix{"basemap", 0, geopackage::Size<int>{2, 2}, geopackage::Size<int>{256, 256}, geopackage::Size<double>{2.0, 2.0}});
    std::optional<geopackage::TileMatrix> s2 = geopackage.getTileMatrix("basemap", 0);
    EXPECT_TRUE(s2.has_value());
    EXPECT_EQ(2, s2.value().getMatrixSize().getWidth());

    EXPECT_TRUE(std::filesystem::remove(fileName));
}

TEST(GeoPackageLibTests, GeoPackage_Delete_TileMatrix) {
    const std::string fileName = "data.gpkg";
    geopackage::GeoPackage geopackage { fileName };
    EXPECT_TRUE(std::filesystem::exists(fileName));

    geopackage.addTileMatrix(geopackage::TileMatrix{"basemap", 0, geopackage::Size<int>{2, 2}, geopackage::Size<int>{256, 256}, geopackage::Size<double>{2.0, 2.0}});
    std::optional<geopackage::TileMatrix> s1 = geopackage.getTileMatrix("basemap", 0);
    EXPECT_TRUE(s1.has_value());
    EXPECT_EQ(2, s1.value().getMatrixSize().getWidth());

    geopackage.deleteTileMatrix(s1.value());
    std::optional<geopackage::TileMatrix> s2 = geopackage.getTileMatrix("basemap", 0);
    EXPECT_FALSE(s2.has_value());

    EXPECT_TRUE(std::filesystem::remove(fileName));
}

TEST(GeoPackageLibTests, GeoPackage_Get_TileMatrix) {
    const std::string fileName = "data.gpkg";
    geopackage::GeoPackage geopackage { fileName };
    EXPECT_TRUE(std::filesystem::exists(fileName));

    std::optional<geopackage::TileMatrix> s1 = geopackage.getTileMatrix("basemap", 0);
    EXPECT_FALSE(s1.has_value());
    geopackage.addTileMatrix(geopackage::TileMatrix{"basemap", 0, geopackage::Size<int>{2, 2}, geopackage::Size<int>{256, 256}, geopackage::Size<double>{2.0, 2.0}});
    std::optional<geopackage::TileMatrix> s2 = geopackage.getTileMatrix("basemap", 0);
    EXPECT_TRUE(s2.has_value());

    EXPECT_TRUE(std::filesystem::remove(fileName));
}

TEST(GeoPackageLibTests, GeoPackage_All_TileMatrices) {
    const std::string fileName = "data.gpkg";
    geopackage::GeoPackage geopackage { fileName };
    EXPECT_TRUE(std::filesystem::exists(fileName));

    geopackage.addTileMatrix(geopackage::TileMatrix{"basemap", 0, geopackage::Size<int>{2, 2}, geopackage::Size<int>{256, 256}, geopackage::Size<double>{2.0, 2.0}});
    geopackage.addTileMatrix(geopackage::TileMatrix{"basemap", 1, geopackage::Size<int>{4, 4}, geopackage::Size<int>{256, 256}, geopackage::Size<double>{1.0, 1.0}});
    geopackage.addTileMatrix(geopackage::TileMatrix{"basemap", 2, geopackage::Size<int>{8, 8}, geopackage::Size<int>{256, 256}, geopackage::Size<double>{0.5, 0.5}});
    
    int counter = 0;    
    geopackage.tileMatrices([&](geopackage::TileMatrix& s) {
        counter++;
    });
    EXPECT_EQ(3, counter);


    EXPECT_TRUE(std::filesystem::remove(fileName));
}

TEST(GeoPackageLibTests, GeoPackage_TileMatrices_Layer) {
    const std::string fileName = "data.gpkg";
    geopackage::GeoPackage geopackage { fileName };
    EXPECT_TRUE(std::filesystem::exists(fileName));

    geopackage.addTileMatrix(geopackage::TileMatrix{"basemap", 0, geopackage::Size<int>{2, 2}, geopackage::Size<int>{256, 256}, geopackage::Size<double>{2.0, 2.0}});
    geopackage.addTileMatrix(geopackage::TileMatrix{"basemap", 1, geopackage::Size<int>{4, 4}, geopackage::Size<int>{256, 256}, geopackage::Size<double>{1.0, 1.0}});
    geopackage.addTileMatrix(geopackage::TileMatrix{"basemap", 2, geopackage::Size<int>{8, 8}, geopackage::Size<int>{256, 256}, geopackage::Size<double>{0.5, 0.5}});

    geopackage.addTileMatrix(geopackage::TileMatrix{"tiles", 0, geopackage::Size<int>{2, 2}, geopackage::Size<int>{256, 256}, geopackage::Size<double>{2.0, 2.0}});
    geopackage.addTileMatrix(geopackage::TileMatrix{"tiles", 1, geopackage::Size<int>{4, 4}, geopackage::Size<int>{256, 256}, geopackage::Size<double>{1.0, 1.0}});
    geopackage.addTileMatrix(geopackage::TileMatrix{"tiles", 2, geopackage::Size<int>{8, 8}, geopackage::Size<int>{256, 256}, geopackage::Size<double>{0.5, 0.5}});
    
    int counter = 0;    
    geopackage.tileMatrices("tiles", [&](geopackage::TileMatrix& s) {
        counter++;
    });
    EXPECT_EQ(3, counter);


    EXPECT_TRUE(std::filesystem::remove(fileName));
}
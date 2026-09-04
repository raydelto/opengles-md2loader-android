#!/usr/bin/env python3
import os
import uuid

def gen_id():
    return uuid.uuid4().hex[:24].upper()

def main():
    project_dir = os.path.abspath("ios")
    os.makedirs(os.path.join(project_dir, "MD2Loader.xcodeproj"), exist_ok=True)
    pbx_path = os.path.join(project_dir, "MD2Loader.xcodeproj", "project.pbxproj")

    # IDs
    id_proj = "100000000000000000000001"
    id_target = "200000000000000000000001"
    id_main_group = "300000000000000000000001"
    id_products_group = "300000000000000000000002"
    id_app_group = "300000000000000000000003"
    id_cpp_group = "300000000000000000000004"
    id_assets_group = "300000000000000000000005"
    id_frameworks_group = "300000000000000000000006"

    id_sources_phase = "400000000000000000000001"
    id_frameworks_phase = "400000000000000000000002"
    id_resources_phase = "400000000000000000000003"

    id_app_product = "500000000000000000000001"

    id_cfg_proj_debug = "600000000000000000000001"
    id_cfg_proj_release = "600000000000000000000002"
    id_cfg_target_debug = "600000000000000000000003"
    id_cfg_target_release = "600000000000000000000004"
    id_cfglist_proj = "700000000000000000000001"
    id_cfglist_target = "700000000000000000000002"

    # Framework references
    id_ref_opengles = "800000000000000000000001"
    id_bld_opengles = "800000000000000000000002"
    id_ref_glkit = "800000000000000000000003"
    id_bld_glkit = "800000000000000000000004"
    id_ref_uikit = "800000000000000000000005"
    id_bld_uikit = "800000000000000000000006"

    # Source files in MD2Loader/
    swift_and_objc = [
        ("AppDelegate.swift", "sourcecode.swift", True, False),
        ("SceneDelegate.swift", "sourcecode.swift", True, False),
        ("MainActivity.swift", "sourcecode.swift", True, False),
        ("MyRenderer.swift", "sourcecode.swift", True, False),
        ("MD2RendererBridge.h", "sourcecode.c.h", False, False),
        ("MD2RendererBridge.mm", "sourcecode.cpp.objcpp", True, False),
        ("MD2Loader-Bridging-Header.h", "sourcecode.c.h", False, False),
        ("Info.plist", "text.plist.xml", False, False),
        ("Assets.xcassets", "folder.assetcatalog", False, True),
    ]

    # Shared C++ files from ../app/src/main/cpp/opengl
    cpp_files = [
        ("Renderer.cpp", "sourcecode.cpp.cpp", True, False),
        ("Renderer.h", "sourcecode.c.h", False, False),
        ("MD2Model.cpp", "sourcecode.cpp.cpp", True, False),
        ("MD2Model.h", "sourcecode.c.h", False, False),
        ("ShaderProgram.cpp", "sourcecode.cpp.cpp", True, False),
        ("ShaderProgram.h", "sourcecode.c.h", False, False),
        ("Texture2D.cpp", "sourcecode.cpp.cpp", True, False),
        ("Texture2D.h", "sourcecode.c.h", False, False),
        ("Logger.h", "sourcecode.c.h", False, False),
    ]

    # Assets from ../app/src/main/assets
    asset_files = [
        ("basic.vert", "text", False, True),
        ("basic.frag", "text", False, True),
        ("female.md2", "file", False, True),
        ("female.tga", "image.tga", False, True),
        ("grunt.md2", "file", False, True),
        ("grunt.tga", "image.tga", False, True),
    ]

    file_entries = []
    # (ref_id, bld_id, name, path, file_type, is_source, is_resource, group_id)
    cur_idx = 10
    for name, ftype, is_src, is_res in swift_and_objc:
        ref_id = f"A0000000000000000000{cur_idx:04d}"
        bld_id = f"B0000000000000000000{cur_idx:04d}" if (is_src or is_res) else None
        cur_idx += 1
        path = f"MD2Loader/{name}"
        file_entries.append((ref_id, bld_id, name, path, ftype, is_src, is_res, id_app_group))

    for name, ftype, is_src, is_res in cpp_files:
        ref_id = f"A0000000000000000000{cur_idx:04d}"
        bld_id = f"B0000000000000000000{cur_idx:04d}" if (is_src or is_res) else None
        cur_idx += 1
        path = f"../app/src/main/cpp/opengl/{name}"
        file_entries.append((ref_id, bld_id, name, path, ftype, is_src, is_res, id_cpp_group))

    for name, ftype, is_src, is_res in asset_files:
        ref_id = f"A0000000000000000000{cur_idx:04d}"
        bld_id = f"B0000000000000000000{cur_idx:04d}" if (is_src or is_res) else None
        cur_idx += 1
        path = f"../app/src/main/assets/{name}"
        file_entries.append((ref_id, bld_id, name, path, ftype, is_src, is_res, id_assets_group))

    # Build file sections
    pbx_build_files = []
    # Frameworks
    pbx_build_files.append(f"\t\t{id_bld_opengles} /* OpenGLES.framework in Frameworks */ = {{isa = PBXBuildFile; fileRef = {id_ref_opengles} /* OpenGLES.framework */; }};")
    pbx_build_files.append(f"\t\t{id_bld_glkit} /* GLKit.framework in Frameworks */ = {{isa = PBXBuildFile; fileRef = {id_ref_glkit} /* GLKit.framework */; }};")
    pbx_build_files.append(f"\t\t{id_bld_uikit} /* UIKit.framework in Frameworks */ = {{isa = PBXBuildFile; fileRef = {id_ref_uikit} /* UIKit.framework */; }};")

    for ref_id, bld_id, name, path, ftype, is_src, is_res, gid in file_entries:
        if is_src:
            pbx_build_files.append(f"\t\t{bld_id} /* {name} in Sources */ = {{isa = PBXBuildFile; fileRef = {ref_id} /* {name} */; }};")
        elif is_res:
            pbx_build_files.append(f"\t\t{bld_id} /* {name} in Resources */ = {{isa = PBXBuildFile; fileRef = {ref_id} /* {name} */; }};")

    # File references
    pbx_file_refs = []
    pbx_file_refs.append(f"\t\t{id_app_product} /* MD2Loader.app */ = {{isa = PBXFileReference; explicitFileType = wrapper.application; includeInIndex = 0; path = MD2Loader.app; sourceTree = BUILT_PRODUCTS_DIR; }};")
    pbx_file_refs.append(f"\t\t{id_ref_opengles} /* OpenGLES.framework */ = {{isa = PBXFileReference; lastKnownFileType = wrapper.framework; name = OpenGLES.framework; path = System/Library/Frameworks/OpenGLES.framework; sourceTree = SDKROOT; }};")
    pbx_file_refs.append(f"\t\t{id_ref_glkit} /* GLKit.framework */ = {{isa = PBXFileReference; lastKnownFileType = wrapper.framework; name = GLKit.framework; path = System/Library/Frameworks/GLKit.framework; sourceTree = SDKROOT; }};")
    pbx_file_refs.append(f"\t\t{id_ref_uikit} /* UIKit.framework */ = {{isa = PBXFileReference; lastKnownFileType = wrapper.framework; name = UIKit.framework; path = System/Library/Frameworks/UIKit.framework; sourceTree = SDKROOT; }};")

    for ref_id, bld_id, name, path, ftype, is_src, is_res, gid in file_entries:
        pbx_file_refs.append(f"\t\t{ref_id} /* {name} */ = {{isa = PBXFileReference; lastKnownFileType = {ftype}; name = \"{name}\"; path = \"{path}\"; sourceTree = \"<group>\"; }};")

    # Sources build phase
    sources_files = []
    for ref_id, bld_id, name, path, ftype, is_src, is_res, gid in file_entries:
        if is_src:
            sources_files.append(f"\t\t\t\t{bld_id} /* {name} in Sources */,")

    # Resources build phase
    resources_files = []
    for ref_id, bld_id, name, path, ftype, is_src, is_res, gid in file_entries:
        if is_res:
            resources_files.append(f"\t\t\t\t{bld_id} /* {name} in Resources */,")

    # Group children
    app_children = [f"\t\t\t\t{entry[0]} /* {entry[2]} */," for entry in file_entries if entry[7] == id_app_group]
    cpp_children = [f"\t\t\t\t{entry[0]} /* {entry[2]} */," for entry in file_entries if entry[7] == id_cpp_group]
    asset_children = [f"\t\t\t\t{entry[0]} /* {entry[2]} */," for entry in file_entries if entry[7] == id_assets_group]

    content = f"""// !$*UTF8*$!
{{
\tarchiveVersion = 1;
\tclasses = {{
\t}};
\tobjectVersion = 56;
\tobjects = {{

/* Begin PBXBuildFile section */
{os.linesep.join(pbx_build_files)}
/* End PBXBuildFile section */

/* Begin PBXFileReference section */
{os.linesep.join(pbx_file_refs)}
/* End PBXFileReference section */

/* Begin PBXFrameworksBuildPhase section */
\t\t{id_frameworks_phase} /* Frameworks */ = {{
\t\t\tisa = PBXFrameworksBuildPhase;
\t\t\tbuildActionMask = 2147483647;
\t\t\tfiles = (
\t\t\t\t{id_bld_opengles} /* OpenGLES.framework in Frameworks */,
\t\t\t\t{id_bld_glkit} /* GLKit.framework in Frameworks */,
\t\t\t\t{id_bld_uikit} /* UIKit.framework in Frameworks */,
\t\t\t);
\t\t\trunOnlyForDeploymentPostprocessing = 0;
\t\t}};
/* End PBXFrameworksBuildPhase section */

/* Begin PBXGroup section */
\t\t{id_main_group} = {{
\t\t\tisa = PBXGroup;
\t\t\tchildren = (
\t\t\t\t{id_app_group} /* MD2Loader */,
\t\t\t\t{id_cpp_group} /* C++ Core */,
\t\t\t\t{id_assets_group} /* Shared Assets */,
\t\t\t\t{id_frameworks_group} /* Frameworks */,
\t\t\t\t{id_products_group} /* Products */,
\t\t\t);
\t\t\tsourceTree = "<group>";
\t\t}};
\t\t{id_app_group} /* MD2Loader */ = {{
\t\t\tisa = PBXGroup;
\t\t\tchildren = (
{os.linesep.join(app_children)}
\t\t\t);
\t\t\tname = MD2Loader;
\t\t\tsourceTree = "<group>";
\t\t}};
\t\t{id_cpp_group} /* C++ Core */ = {{
\t\t\tisa = PBXGroup;
\t\t\tchildren = (
{os.linesep.join(cpp_children)}
\t\t\t);
\t\t\tname = "C++ Core";
\t\t\tsourceTree = "<group>";
\t\t}};
\t\t{id_assets_group} /* Shared Assets */ = {{
\t\t\tisa = PBXGroup;
\t\t\tchildren = (
{os.linesep.join(asset_children)}
\t\t\t);
\t\t\tname = "Shared Assets";
\t\t\tsourceTree = "<group>";
\t\t}};
\t\t{id_frameworks_group} /* Frameworks */ = {{
\t\t\tisa = PBXGroup;
\t\t\tchildren = (
\t\t\t\t{id_ref_opengles} /* OpenGLES.framework */,
\t\t\t\t{id_ref_glkit} /* GLKit.framework */,
\t\t\t\t{id_ref_uikit} /* UIKit.framework */,
\t\t\t);
\t\t\tname = Frameworks;
\t\t\tsourceTree = "<group>";
\t\t}};
\t\t{id_products_group} /* Products */ = {{
\t\t\tisa = PBXGroup;
\t\t\tchildren = (
\t\t\t\t{id_app_product} /* MD2Loader.app */,
\t\t\t);
\t\t\tname = Products;
\t\t\tsourceTree = "<group>";
\t\t}};
/* End PBXGroup section */

/* Begin PBXNativeTarget section */
\t\t{id_target} /* MD2Loader */ = {{
\t\t\tisa = PBXNativeTarget;
\t\t\tbuildConfigurationList = {id_cfglist_target} /* Build configuration list for PBXNativeTarget "MD2Loader" */;
\t\t\tbuildPhases = (
\t\t\t\t{id_sources_phase} /* Sources */,
\t\t\t\t{id_frameworks_phase} /* Frameworks */,
\t\t\t\t{id_resources_phase} /* Resources */,
\t\t\t);
\t\t\tbuildRules = (
\t\t\t);
\t\t\tdependencies = (
\t\t\t);
\t\t\tname = MD2Loader;
\t\t\tproductName = MD2Loader;
\t\t\tproductReference = {id_app_product} /* MD2Loader.app */;
\t\t\tproductType = "com.apple.product-type.application";
\t\t}};
/* End PBXNativeTarget section */

/* Begin PBXProject section */
\t\t{id_proj} /* Project object */ = {{
\t\t\tisa = PBXProject;
\t\t\tattributes = {{
\t\t\t\tBuildIndependentTargetsInParallel = 1;
\t\t\t\tLastSwiftUpdateCheck = 1600;
\t\t\t\tLastUpgradeCheck = 1600;
\t\t\t\tTargetAttributes = {{
\t\t\t\t\t{id_target} = {{
\t\t\t\t\t\tCreatedOnToolsVersion = 16.0;
\t\t\t\t\t\tDevelopmentTeam = XDX46V68VR;
\t\t\t\t\t\tProvisioningStyle = Automatic;
\t\t\t\t\t}};
\t\t\t\t}};
\t\t\t}};
\t\t\tbuildConfigurationList = {id_cfglist_proj} /* Build configuration list for PBXProject "MD2Loader" */;
\t\t\tcompatibilityVersion = "Xcode 14.0";
\t\t\tdevelopmentRegion = en;
\t\t\thasScannedForEncodings = 0;
\t\t\tknownRegions = (
\t\t\t\ten,
\t\t\t\tBase,
\t\t\t);
\t\t\tmainGroup = {id_main_group};
\t\t\tproductRefGroup = {id_products_group} /* Products */;
\t\t\tprojectDirPath = "";
\t\t\tprojectRoot = "";
\t\t\ttargets = (
\t\t\t\t{id_target} /* MD2Loader */,
\t\t\t);
\t\t}};
/* End PBXProject section */

/* Begin PBXResourcesBuildPhase section */
\t\t{id_resources_phase} /* Resources */ = {{
\t\t\tisa = PBXResourcesBuildPhase;
\t\t\tbuildActionMask = 2147483647;
\t\t\tfiles = (
{os.linesep.join(resources_files)}
\t\t\t);
\t\t\trunOnlyForDeploymentPostprocessing = 0;
\t\t}};
/* End PBXResourcesBuildPhase section */

/* Begin PBXSourcesBuildPhase section */
\t\t{id_sources_phase} /* Sources */ = {{
\t\t\tisa = PBXSourcesBuildPhase;
\t\t\tbuildActionMask = 2147483647;
\t\t\tfiles = (
{os.linesep.join(sources_files)}
\t\t\t);
\t\t\trunOnlyForDeploymentPostprocessing = 0;
\t\t}};
/* End PBXSourcesBuildPhase section */

/* Begin XCBuildConfiguration section */
\t\t{id_cfg_proj_debug} /* Debug */ = {{
\t\t\tisa = XCBuildConfiguration;
\t\t\tbuildSettings = {{
\t\t\t\tALWAYS_SEARCH_USER_PATHS = NO;
\t\t\t\tCLANG_ANALYZER_NONNULL = YES;
\t\t\t\tCLANG_CXX_LANGUAGE_STANDARD = "gnu++17";
\t\t\t\tCLANG_ENABLE_MODULES = YES;
\t\t\t\tCLANG_ENABLE_OBJC_ARC = YES;
\t\t\t\tCOPY_PHASE_STRIP = NO;
\t\t\t\tDEBUG_INFORMATION_FORMAT = dwarf;
\t\t\t\tENABLE_STRICT_OBJC_MSGSEND = YES;
\t\t\t\tENABLE_TESTABILITY = YES;
\t\t\t\tGCC_DYNAMIC_NO_PIC = NO;
\t\t\t\tGCC_NO_COMMON_BLOCKS = YES;
\t\t\t\tGCC_OPTIMIZATION_LEVEL = 0;
\t\t\t\tGCC_PREPROCESSOR_DEFINITIONS = (
\t\t\t\t\t"DEBUG=1",
\t\t\t\t\t"$(inherited)",
\t\t\t\t\t"GLES_SILENCE_DEPRECATION=1",
\t\t\t\t);
\t\t\t\tGCC_WARN_64_TO_32_BIT_CONVERSION = YES;
\t\t\t\tGCC_WARN_ABOUT_RETURN_TYPE = YES_ERROR;
\t\t\t\tGCC_WARN_UNDEFINED_VARIABLES = YES;
\t\t\t\tGCC_WARN_UNINITIALIZED_AUTOS = YES_AGGRESSIVE;
\t\t\t\tIPHONEOS_DEPLOYMENT_TARGET = 17.0;
\t\t\t\tMTL_ENABLE_DEBUG_INFO = INCLUDE_SOURCE;
\t\t\t\tMTL_FAST_MATH = YES;
\t\t\t\tONLY_ACTIVE_ARCH = YES;
\t\t\t\tSDKROOT = iphoneos;
\t\t\t\tSWIFT_ACTIVE_COMPILATION_CONDITIONS = DEBUG;
\t\t\t\tSWIFT_OPTIMIZATION_LEVEL = "-Onone";
\t\t\t}};
\t\t\tname = Debug;
\t\t}};
\t\t{id_cfg_proj_release} /* Release */ = {{
\t\t\tisa = XCBuildConfiguration;
\t\t\tbuildSettings = {{
\t\t\t\tALWAYS_SEARCH_USER_PATHS = NO;
\t\t\t\tCLANG_ANALYZER_NONNULL = YES;
\t\t\t\tCLANG_CXX_LANGUAGE_STANDARD = "gnu++17";
\t\t\t\tCLANG_ENABLE_MODULES = YES;
\t\t\t\tCLANG_ENABLE_OBJC_ARC = YES;
\t\t\t\tCOPY_PHASE_STRIP = NO;
\t\t\t\tDEBUG_INFORMATION_FORMAT = "dwarf-with-dsym";
\t\t\t\tENABLE_NS_ASSERTIONS = NO;
\t\t\t\tENABLE_STRICT_OBJC_MSGSEND = YES;
\t\t\t\tGCC_NO_COMMON_BLOCKS = YES;
\t\t\t\tGCC_PREPROCESSOR_DEFINITIONS = (
\t\t\t\t\t"$(inherited)",
\t\t\t\t\t"GLES_SILENCE_DEPRECATION=1",
\t\t\t\t);
\t\t\t\tGCC_WARN_64_TO_32_BIT_CONVERSION = YES;
\t\t\t\tGCC_WARN_ABOUT_RETURN_TYPE = YES_ERROR;
\t\t\t\tGCC_WARN_UNDEFINED_VARIABLES = YES;
\t\t\t\tGCC_WARN_UNINITIALIZED_AUTOS = YES_AGGRESSIVE;
\t\t\t\tIPHONEOS_DEPLOYMENT_TARGET = 17.0;
\t\t\t\tMTL_ENABLE_DEBUG_INFO = NO;
\t\t\t\tMTL_FAST_MATH = YES;
\t\t\t\tSDKROOT = iphoneos;
\t\t\t\tSWIFT_COMPILATION_MODE = wholemodule;
\t\t\t\tSWIFT_OPTIMIZATION_LEVEL = "-O";
\t\t\t\tVALIDATE_PRODUCT = YES;
\t\t\t}};
\t\t\tname = Release;
\t\t}};
\t\t{id_cfg_target_debug} /* Debug */ = {{
\t\t\tisa = XCBuildConfiguration;
\t\t\tbuildSettings = {{
\t\t\t\tASSETCATALOG_COMPILER_APPICON_NAME = AppIcon;
\t\t\t\tASSETCATALOG_COMPILER_GLOBAL_ACCENT_COLOR_NAME = AccentColor;
\t\t\t\tCODE_SIGN_STYLE = Automatic;
\t\t\t\tCURRENT_PROJECT_VERSION = 1;
\t\t\t\tDEVELOPMENT_TEAM = XDX46V68VR;
\t\t\t\tGENERATE_INFOPLIST_FILE = NO;
\t\t\t\tHEADER_SEARCH_PATHS = (
\t\t\t\t\t"$(inherited)",
\t\t\t\t\t"$(PROJECT_DIR)/../app/src/main/cpp/opengl",
\t\t\t\t\t"$(PROJECT_DIR)/../app/src/main/cpp/third_party_libraries",
\t\t\t\t);
\t\t\t\tINFOPLIST_FILE = MD2Loader/Info.plist;
\t\t\t\tIPHONEOS_DEPLOYMENT_TARGET = 17.0;
\t\t\t\tLD_RUNPATH_SEARCH_PATHS = (
\t\t\t\t\t"$(inherited)",
\t\t\t\t\t"@executable_path/Frameworks",
\t\t\t\t);
\t\t\t\tMARKETING_VERSION = 1.0;
\t\t\t\tPRODUCT_BUNDLE_IDENTIFIER = com.raydelto.MD2Loader;
\t\t\t\tPRODUCT_NAME = "$(TARGET_NAME)";
\t\t\t\tSWIFT_EMIT_LOC_STRINGS = YES;
\t\t\t\tSWIFT_OBJC_BRIDGING_HEADER = "MD2Loader/MD2Loader-Bridging-Header.h";
\t\t\t\tSWIFT_VERSION = 5.0;
\t\t\t\tTARGETED_DEVICE_FAMILY = "1,2";
\t\t\t}};
\t\t\tname = Debug;
\t\t}};
\t\t{id_cfg_target_release} /* Release */ = {{
\t\t\tisa = XCBuildConfiguration;
\t\t\tbuildSettings = {{
\t\t\t\tASSETCATALOG_COMPILER_APPICON_NAME = AppIcon;
\t\t\t\tASSETCATALOG_COMPILER_GLOBAL_ACCENT_COLOR_NAME = AccentColor;
\t\t\t\tCODE_SIGN_STYLE = Automatic;
\t\t\t\tCURRENT_PROJECT_VERSION = 1;
\t\t\t\tDEVELOPMENT_TEAM = XDX46V68VR;
\t\t\t\tGENERATE_INFOPLIST_FILE = NO;
\t\t\t\tHEADER_SEARCH_PATHS = (
\t\t\t\t\t"$(inherited)",
\t\t\t\t\t"$(PROJECT_DIR)/../app/src/main/cpp/opengl",
\t\t\t\t\t"$(PROJECT_DIR)/../app/src/main/cpp/third_party_libraries",
\t\t\t\t);
\t\t\t\tINFOPLIST_FILE = MD2Loader/Info.plist;
\t\t\t\tIPHONEOS_DEPLOYMENT_TARGET = 17.0;
\t\t\t\tLD_RUNPATH_SEARCH_PATHS = (
\t\t\t\t\t"$(inherited)",
\t\t\t\t\t"@executable_path/Frameworks",
\t\t\t\t);
\t\t\t\tMARKETING_VERSION = 1.0;
\t\t\t\tPRODUCT_BUNDLE_IDENTIFIER = com.raydelto.MD2Loader;
\t\t\t\tPRODUCT_NAME = "$(TARGET_NAME)";
\t\t\t\tSWIFT_EMIT_LOC_STRINGS = YES;
\t\t\t\tSWIFT_OBJC_BRIDGING_HEADER = "MD2Loader/MD2Loader-Bridging-Header.h";
\t\t\t\tSWIFT_VERSION = 5.0;
\t\t\t\tTARGETED_DEVICE_FAMILY = "1,2";
\t\t\t}};
\t\t\tname = Release;
\t\t}};
/* End XCBuildConfiguration section */

/* Begin XCConfigurationList section */
\t\t{id_cfglist_proj} /* Build configuration list for PBXProject "MD2Loader" */ = {{
\t\t\tisa = XCConfigurationList;
\t\t\tbuildConfigurations = (
\t\t\t\t{id_cfg_proj_debug} /* Debug */,
\t\t\t\t{id_cfg_proj_release} /* Release */,
\t\t\t);
\t\t\tdefaultConfigurationIsVisible = 0;
\t\t\tdefaultConfigurationName = Release;
\t\t}};
\t\t{id_cfglist_target} /* Build configuration list for PBXNativeTarget "MD2Loader" */ = {{
\t\t\tisa = XCConfigurationList;
\t\t\tbuildConfigurations = (
\t\t\t\t{id_cfg_target_debug} /* Debug */,
\t\t\t\t{id_cfg_target_release} /* Release */,
\t\t\t);
\t\t\tdefaultConfigurationIsVisible = 0;
\t\t\tdefaultConfigurationName = Release;
\t\t}};
/* End XCConfigurationList section */
\t}};
\trootObject = {id_proj} /* Project object */;
}}
"""
    with open(pbx_path, "w") as f:
        f.write(content)
    print(f"Generated {pbx_path} successfully!")

if __name__ == "__main__":
    main()

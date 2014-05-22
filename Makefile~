
include $(TOPDIR)/rules.mk

PKG_NAME:=aplay
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

.
define Package/aplay
	SECTION:=sound
	CATEGORY:=Sound
	TITLE:=aplay_new	

endef

 
define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Package/aplay/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/aplay $(1)/bin/
endef

$(eval $(call BuildPackage,aplay))

